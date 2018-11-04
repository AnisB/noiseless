#pragma once

// Library include
#include "bento_math/types.h"
#include "bento_collection/vector.h"

namespace noiseless
{
	struct TMesh
	{
		ALLOCATOR_BASED;
		TMesh(bento::IAllocator& allocator)
		: vertices(allocator)
		, indices(allocator)
		{
		}
		
		bento::Vector<bento::Vector3> vertices;
		bento::Vector<bento::IVector3> indices;
	};
}