#pragma once

// Internal includes
#include "gpu_backend/compute_api.h"
#include "model/scene.h"

namespace noiseless
{
	struct TComputeScene
	{
		ComputeBuffer vertices;
		ComputeBuffer indices;
	};

	// Request a pointer to the compute scene
	void create_compute_scene(ComputeContext computeContext, const TScene& scene, TComputeScene& outputComputeScene);
	void destroy_compute_scene(TComputeScene& targetComputeScene);
}