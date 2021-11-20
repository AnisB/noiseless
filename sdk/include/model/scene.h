#pragma once

// Internal includes
#include "mesh.h"

namespace noiseless
{
	class TScene
	{
	public:
		TScene(bento::IAllocator& allocator);

		// Resets the number of meshes for this scene
		void set_num_meshes(uint32_t numMeshes);
		uint32_t num_meshes() const { return _meshes.size(); }

		// Access a target mesh using its index
		TMesh& mesh(uint32_t meshIndex) { return _meshes[meshIndex]; }
		const TMesh& mesh(uint32_t meshIndex) const { return _meshes[meshIndex]; }

	private:
		// The list of meshes in the scene
		bento::Vector<TMesh> _meshes;

	public:
		bento::IAllocator& allocator;
	};
}