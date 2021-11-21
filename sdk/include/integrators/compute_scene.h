#pragma once
// bento includes
#include <bento_compute/compute_api.h>

// Internal includes
#include "model/scene.h"

namespace noiseless
{
	struct TComputeScene
	{
		TComputeScene()
		: vertices(0)
		, indices(0)
		, bvhNodes(0)
		, bvhPrimitives(0)
		{
		}

		// Geometry data
		bento::ComputeBuffer vertices;
		bento::ComputeBuffer indices;
		uint32_t numTriangles;

		// BVH data
		bento::ComputeBuffer bvhNodes;
		bento::ComputeBuffer bvhPrimitives;
	};

	// Request a pointer to the compute scene
	void create_compute_scene(bento::ComputeContext computeContext, bento::ComputeCommandList commandList, const TScene& scene, TComputeScene& outputComputeScene, bento::IAllocator& allocator);
	void destroy_compute_scene(TComputeScene& targetComputeScene);
}