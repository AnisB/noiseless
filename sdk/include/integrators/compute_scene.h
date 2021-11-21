#pragma once
// bento includes
#include <bento_compute/compute_api.h>

// Internal includes
#include "model/scene.h"

namespace noiseless
{
	struct TComputeScene
	{
		// Geometry data
		bento::ComputeBuffer vertices;
		bento::ComputeBuffer indices;

		// BVH data
		bento::ComputeBuffer bvhNodes;
		bento::ComputeBuffer bvhPrimitives;
	};

	// Request a pointer to the compute scene
	void create_compute_scene(bento::ComputeContext computeContext, const TScene& scene, TComputeScene& outputComputeScene, bento::IAllocator& allocator);
	void destroy_compute_scene(TComputeScene& targetComputeScene);
}