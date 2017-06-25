#ifndef BVH_H
#define BVH_H

// Library includes
#include "mesh.h"
#include "box3.h"

// External includes
#include <stdint.h>

namespace noiseless
{
	struct Entry
	{
		uint64_t face_idx;
	};

	struct BVHNode
	{
		Box3 box;
		uint64_t child_or_node_index;
		bool leaf;
	};

	class BVH
	{
	public:
		BVH();
		~BVH();

		void init(const Mesh& mesh, Entry* entry_array, uint64_t num_entries);
	};
}

#endif // BVH_H