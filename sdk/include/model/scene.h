#pragma once

// Internal includes
#include "geometry.h"

namespace noiseless
{
	class TScene
	{
	public:
		TScene(bento::IAllocator& allocator);

		// Resets the number of meshes for this scene
		void set_num_meshes(uint32_t numMeshes);
		uint32_t num_meshes() const { return _geometries.size(); }

		// Access a target mesh using its index
		TGeometry& geometry(uint32_t meshIndex) { return _geometries[meshIndex]; }
		const TGeometry& geometry(uint32_t meshIndex) const { return _geometries[meshIndex]; }

	private:
		// The list of meshes in the scene
		bento::Vector<TGeometry> _geometries;

	public:
		bento::IAllocator& allocator;
	};
}