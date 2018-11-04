// Bento includes
#include <bento_math/matrix4.h>

// Internal includes
#include <model/camera.h>
#include <asset_compiler/compiler.h>
#include <integrators/camera_ray_generation.h>
#include <gpu_backend/compute_api.h>
#include <resources/kernel_source.h>

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
	noiseless::TCompilerOptions options(current_alloc);
	noiseless::asset_compiler::build_from_args(options, arg_array);
	if (options._source_dir == "") return -1;

	// Compile
	bento::TAssetDatabase asset_db(*bento::common_allocator());
	bool result = noiseless::asset_compiler::compile(options, asset_db);

	// Create a fake camera looking forward Z
	noiseless::TCamera camera;
	bento::SetIdentity(camera.view);
	camera.fov = 45;
	camera.nearPlane = 0.001f;
	camera.farPlane = 10000.0f;
	uint32_t width = 1280;
	uint32_t height = 720;

	// The number of rays to generate
	uint32_t numRays = width * height;

	// Build the ray generation settings
	noiseless::TCameraRaySettings cameraRaySettings;
	noiseless::build_ray_generation_data(camera, width, height, cameraRaySettings);

	// Create a gpu context
	noiseless::ComputeContext context = noiseless::create_compute_context();

	// Create an output buffer
	noiseless::ComputeBuffer output_buffer = noiseless::create_buffer(context, numRays * sizeof(noiseless::TRay), noiseless::ComputeBufferType::WRITE_ONLY);

	// Let's request
	noiseless::TKernelSource kernelSource(*bento::common_allocator());
	const bento::TAsset* asset = asset_db.request_asset("kernels/camera_ray_generation.kl");
	const char* assetData = asset->data.begin();
	bento::unpack_type(assetData, kernelSource);

	// Create a program and a kernel
	noiseless::ComputeProgram program = noiseless::create_program_source(context, kernelSource.data.begin());
	noiseless::ComputeKernel kernel = noiseless::create_kernel(program, "camera_ray_generation");

	// Sent the arguments
	noiseless::kernel_argument(kernel, 0, sizeof(noiseless::TCameraRaySettings), (unsigned char*)&cameraRaySettings);
	noiseless::kernel_argument(kernel, 1, output_buffer);
	noiseless::kernel_argument(kernel, 2, sizeof(numRays), (unsigned char*)&numRays);

	// Launch the kernel
	noiseless::launch_kernel_1D(context, kernel, numRays);

	// Wait for the kernel to finish
	noiseless::wait_command_queue(context);

	// Read the output values
	bento::Vector<noiseless::TRay> outputRays(*bento::common_allocator(), numRays);
	noiseless::read_buffer(context, output_buffer, (unsigned char*)&outputRays[0]);

	// Release all the gpu resources
	noiseless::destroy_buffer(output_buffer);
	noiseless::destroy_kernel(kernel);
	noiseless::destroy_program(program);
	noiseless::destroy_compute_context(context);

	return 0;
}