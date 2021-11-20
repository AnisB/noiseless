// Bento includes
#include <bento_math/matrix4.h>
#include <bento_compute/compute_api.h>

// Internal includes
#include <model/camera.h>
#include <asset_compiler/compiler.h>
#include <integrators/camera_ray_generation.h>
#include <resources/kernel_source.h>

using namespace noiseless;

int main(int argc, char** argv)
{
	// Allocator used for this program
	bento::IAllocator& current_alloc = *bento::common_allocator();

	// Collect the arguments
	bento::Vector<bento::DynamicString> arg_array(current_alloc);
	arg_array.resize(argc);
	for (uint32_t arg_idx = 0; arg_idx < (uint32_t)argc; ++arg_idx)
	{
		arg_array[arg_idx] = argv[arg_idx];
	}

	// Build the compiler options
	TCompilerOptions options(current_alloc);
	asset_compiler::build_from_args(options, arg_array);
	if (options._source_dir == "") return -1;

	// Compile
	bento::TAssetDatabase asset_db(*bento::common_allocator());
	bool result = asset_compiler::compile(options, asset_db);

	// Create a fake camera looking forward Z
	TCamera camera;
	bento::SetIdentity(camera.view);
	camera.fov = 45;
	camera.nearPlane = 0.001f;
	camera.farPlane = 10000.0f;
	uint32_t width = 1280;
	uint32_t height = 720;

	// The number of rays to generate
	uint32_t numRays = width * height;

	// Build the ray generation settings
	TCameraRaySettings cameraRaySettings;
	build_ray_generation_data(camera, width, height, cameraRaySettings);

	// Create a compute context
	bento::ComputeContext context = bento::create_compute_context(current_alloc);

	// Create a command list
	bento::ComputeCommandList commandList = bento::create_command_list(context, current_alloc);

	// Create an output buffer
	bento::ComputeBuffer output_buffer = bento::create_buffer(context, numRays * sizeof(TRay), bento::ComputeBufferType::WRITE_ONLY, current_alloc);

	// Let's request
	TKernelSource kernelSource(*bento::common_allocator());
	const bento::TAsset* asset = asset_db.request_asset("kernels/camera_ray_generation.kl");
	const char* assetData = asset->data.begin();
	bento::unpack_type(assetData, kernelSource);

	// Create a program and a kernel
	bento::ComputeProgram program = bento::create_program_source(context, kernelSource.data.begin());
	bento::ComputeKernel kernel = bento::create_kernel(program, "camera_ray_generation");

	// Sent the arguments
	bento::kernel_argument(kernel, 0, sizeof(TCameraRaySettings), (unsigned char*)&cameraRaySettings);
	bento::kernel_argument(kernel, 1, output_buffer);
	bento::kernel_argument(kernel, 2, sizeof(numRays), (unsigned char*)&numRays);

	// Launch the kernel
	bento::dispatch_kernel_1D(commandList, kernel, numRays);

	// Wait for the kernel to finish
	bento::flush_command_list(commandList);

	// Read the output values
	bento::Vector<TRay> outputRays(*bento::common_allocator(), numRays);
	bento::read_buffer(commandList, output_buffer, (unsigned char*)&outputRays[0]);

	// Release all the gpu resources
	bento::destroy_buffer(output_buffer);
	bento::destroy_kernel(kernel);
	bento::destroy_program(program);
	bento::destroy_command_list(commandList);
	bento::destroy_compute_context(context);

	return 0;
}