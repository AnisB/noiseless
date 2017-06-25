// Library includes
#include "bvh.h"

namespace noiseless
{
	BVH::BVH()
	{

	}

	BVH::~BVH()
	{
		
	}

	void BVH::init(const Mesh& mesh, Entry* entry_array, uint64_t num_entries)
	{
		Box3 box;
		box.encapsulate(&mesh.verts[0], mesh.verts.size());
	}
}