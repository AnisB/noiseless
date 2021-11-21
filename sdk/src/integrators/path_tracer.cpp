// Internal includes
#include "integrators/path_tracer.h"
#include "resources/kernel_source.h"
#include "integrators/camera_ray_generation.h"

namespace noiseless
{
	TPathTracer::TPathTracer(bento::IAllocator& p_allocator)
	: allocator(p_allocator)
	, _width(0)
	, _height(0)
	, _computeContext(0)
	, _commandList(0)
	, _computeColorBuffer(0)
	{

	}

	TPathTracer::~TPathTracer()
	{
		release_resources();
	}

	void TPathTracer::init_resources(const bento::TAssetDatabase& assetDatabase)
	{
		// Create the compute context shall be used
		_computeContext = bento::create_compute_context(allocator);

		// Create the command list where we shall enqueue our commands
		_commandList = bento::create_command_list(_computeContext, allocator);

		// Request the path tracer program
		TKernelSource pathTracerSource(allocator);
		assetDatabase.unpack_asset_to_type<TKernelSource>("kernels/path_tracer.kl", pathTracerSource);
		_pathTracerComputeProgram = bento::create_program_source(_computeContext, pathTracerSource.data.begin());
		_pathTracerComputeKernel = bento::create_kernel(_pathTracerComputeProgram, "trace");
	}

	void TPathTracer::setup_scene(const TScene& targetScene)
	{
		create_compute_scene(_computeContext, _commandList, targetScene, _computeScene, allocator);
	}

	void TPathTracer::clear_scene()
	{
		destroy_compute_scene(_computeScene);
	}

	void TPathTracer::release_resources()
	{
		if (_computeColorBuffer != 0)
			bento::destroy_buffer(_computeColorBuffer);

		bento::destroy_kernel(_pathTracerComputeKernel);
		bento::destroy_program(_pathTracerComputeProgram);
		bento::destroy_command_list(_commandList);
		bento::destroy_compute_context(_computeContext);
	}

	void TPathTracer::set_resolution(uint32_t width, uint32_t height)
	{
		// Keep track of the sizes
		_width = width;
		_height = height;
		uint32_t numRays = _width * _height;

		if (_computeColorBuffer != 0)
			bento::destroy_buffer(_computeColorBuffer);
		_computeColorBuffer = bento::create_buffer(_computeContext, numRays * sizeof(bento::Vector4), bento::ComputeBufferType::READ_WRITE, allocator);
	}

	void TPathTracer::render(const TCamera& targetCamera)
	{
		// Let's build the ray settings
		TCameraRaySettings cameraRaySettings;
		build_ray_generation_data(targetCamera, _width, _height, cameraRaySettings);

		// Set the kernel's parameters
		uint32_t numRays = _width * _height;
		bento::kernel_argument(_pathTracerComputeKernel, 0, sizeof(TCameraRaySettings), (unsigned char*)&cameraRaySettings);
		bento::kernel_argument(_pathTracerComputeKernel, 1, _computeScene.vertices);
		bento::kernel_argument(_pathTracerComputeKernel, 2, _computeScene.indices);
		bento::kernel_argument(_pathTracerComputeKernel, 3, sizeof(unsigned int), (unsigned char*)&_computeScene.numTriangles);
		bento::kernel_argument(_pathTracerComputeKernel, 4, _computeColorBuffer);

		// Dispatch the task on the GPU
		bento::IVector3 numTilesParam = { numRays, 1, 1 };
		bento::IVector3 tileSize = { 1, 1, 1 };
		dispatch_kernel(_commandList, _pathTracerComputeKernel, numTilesParam, tileSize);

		// Flush and wait for the command queue
		bento::flush_command_list(_commandList);
	}

	void TPathTracer::read_color_buffer(bento::Vector<bento::Vector4>& outputBuffer) const
	{
		uint32_t numRays = _width * _height;
		if (outputBuffer.size() != numRays)
		{
			outputBuffer.resize(numRays);
		}

		bento::read_buffer(_commandList, _computeColorBuffer, (unsigned char*)&outputBuffer[0]);
	}
}