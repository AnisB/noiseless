#pragma once

// Internal include
#include "integrators/compute_scene.h"
#include "gpu_backend/compute_api.h"
#include "model/camera.h"

// Bento includes
#include <bento_resources/asset_database.h>

namespace noiseless
{
	class TIntegrator
	{
	public:
		TIntegrator(bento::IAllocator& allocator);
		~TIntegrator();

		// Allocate/Release the resources
		void init_resources(const bento::TAssetDatabase& assetDatabase);
		void release_resources();

		// Setup/Clear the scene that should be used for the integrator
		void setup_scene(const TScene& targetScene);
		void clear_scene();

		// Set the resolution of the render target
		void set_resolution(uint32_t width, uint32_t height);

		// Render into the render target from a given camera
		void render(const TCamera& targetCamera);

		// Read back the color buffer
		void read_color_buffer(bento::Vector<bento::Vector4>& outputBuffer) const;

	private:
		// Keep track of the ComputeContext
		ComputeContext _computeContext;

		// Program to generate the rays
		ComputeProgram _rayGenerationProgram;
		ComputeKernel _rayGenerationKernel;

		// Program to evaluate the integration
		ComputeProgram	_integratorComputeProgram;
		ComputeKernel	_integratorComputeKernel;

		// The target scene for the integration
		TComputeScene	_computeScene;

		// The render target for the integrator
		uint32_t _width;
		uint32_t _height;
		ComputeBuffer _computeRayBuffer;
		ComputeBuffer _computeColorBuffer;

	public:
		bento::IAllocator& allocator;
	};
}