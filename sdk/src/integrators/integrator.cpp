// Internal includes
#include "integrators/integrator.h"
#include "resources/kernel_source.h"
#include "gpu_backend/compute_api.h"
#include "integrators/camera_ray_generation.h"

namespace noiseless
{
	TIntegrator::TIntegrator(bento::IAllocator& allocator)
	: allocator(allocator)
	, _width(0)
	, _height(0)
	, _computeContext(0)
	, _commandList(0)
	, _computeRayBuffer(0)
	, _computeColorBuffer(0)
	{

	}

	TIntegrator::~TIntegrator()
	{

	}

	void TIntegrator::init_resources(const bento::TAssetDatabase& assetDatabase)
	{
		// Create the compute context shall be used
		_computeContext = create_compute_context();

		// Create the command list where we shall enqueue our commands
		_commandList = create_command_list(_computeContext);

		// Request the cameray ray generation program and compile it
		TKernelSource rayGenkernelSource(*bento::common_allocator());
		assetDatabase.unpack_asset_to_type<TKernelSource>("kernels/camera_ray_generation.kl", rayGenkernelSource);
		_rayGenerationProgram = create_program_source(_computeContext, rayGenkernelSource.data.begin());
		_rayGenerationKernel = create_kernel(_rayGenerationProgram, "camera_ray_generation");

		// Request the integrator program
		TKernelSource integratorKernelSource(*bento::common_allocator());
		assetDatabase.unpack_asset_to_type<TKernelSource>("kernels/integrator.kl", integratorKernelSource);
		_integratorComputeProgram = create_program_source(_computeContext, integratorKernelSource.data.begin());
		_integratorComputeKernel = create_kernel(_integratorComputeProgram, "trace");
	}

	void TIntegrator::setup_scene(const TScene& targetScene)
	{
		create_compute_scene(_computeContext, targetScene, _computeScene);
	}

	void TIntegrator::clear_scene()
	{
		destroy_compute_scene(_computeScene);
	}

	void TIntegrator::release_resources()
	{
		destroy_kernel(_integratorComputeKernel);
		destroy_program(_integratorComputeProgram);

		destroy_kernel(_rayGenerationKernel);
		destroy_program(_rayGenerationProgram);

		destroy_command_list(_commandList);

		destroy_compute_context(_computeContext);
	}

	void TIntegrator::set_resolution(uint32_t width, uint32_t height)
	{
		// Keep track of the sizes
		_width = width;
		_height = height;
		uint32_t numRays = _width * _height;

		// Clean and allocate the ray and color buffers
		if (_computeRayBuffer != 0)
		{
			destroy_buffer(_computeRayBuffer);
			_computeRayBuffer = 0;
		}
		create_buffer(_computeContext, numRays * sizeof(TRay), ComputeBufferType::READ_WRITE);

		if (_computeColorBuffer != 0)
		{
			destroy_buffer(_computeColorBuffer);
			_computeColorBuffer = 0;
		}
		create_buffer(_computeColorBuffer, numRays * sizeof(bento::Vector4), ComputeBufferType::READ_WRITE);
		create_buffer(_computeContext, numRays * sizeof(TRay), ComputeBufferType::WRITE_ONLY);
	}

	void TIntegrator::render(const TCamera& targetCamera)
	{
		// Let's build the ray settings
		TCameraRaySettings cameraRaySettings;
		build_ray_generation_data(targetCamera, _width, _height, cameraRaySettings);

		// Set the kernel's parameters
		uint32_t numRays = _width * _height;
		kernel_argument(_rayGenerationKernel, 0, sizeof(TCameraRaySettings), (unsigned char*)&cameraRaySettings);
		kernel_argument(_rayGenerationKernel, 1, _computeRayBuffer);
		kernel_argument(_rayGenerationKernel, 2, sizeof(numRays), (unsigned char*)&numRays);

		// Push the ray generation command
		dispatch_kernel_1D(_commandList, _rayGenerationKernel, numRays);

		// Now that the ray generation is done, let's execute the integration evaluation
		kernel_argument(_integratorComputeKernel, 0, _computeRayBuffer);
		kernel_argument(_integratorComputeKernel, 1, sizeof(numRays), (unsigned char*)&numRays);
		kernel_argument(_integratorComputeKernel, 2, _computeColorBuffer);

		// Push the ray iintegration evaluation kernel
		dispatch_kernel_1D(_commandList, _integratorComputeKernel, numRays);

		// Flush and wait for the command queue
		flush_command_list(_commandList);
	}

	void TIntegrator::read_color_buffer(bento::Vector<bento::Vector4>& outputBuffer) const
	{
		uint32_t numRays = _width * _height;
		if (outputBuffer.size() != numRays)
		{
			outputBuffer.resize(numRays);
		}

		read_buffer(_commandList, _computeColorBuffer, (unsigned char*)&outputBuffer[0]);
	}
}