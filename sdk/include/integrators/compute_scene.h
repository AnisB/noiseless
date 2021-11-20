#pragma once
// bento includes
#include <bento_compute/compute_api.h>

// Internal includes
#include "model/scene.h"

namespace noiseless
{
	struct TComputeScene
	{
		bento::ComputeBuffer BVH;
		bento::ComputeBuffer vertices;
		bento::ComputeBuffer indices;
	};

	// Request a pointer to the compute scene
	void create_compute_scene(bento::ComputeContext computeContext, const TScene& scene, TComputeScene& outputComputeScene, bento::IAllocator& allocator);
	void destroy_compute_scene(TComputeScene& targetComputeScene);
}