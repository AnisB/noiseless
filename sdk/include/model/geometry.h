#pragma once

// Library include
#include "bento_math/types.h"
#include "bento_collection/vector.h"
#include "bento_math/matrix4.h"

namespace noiseless
{
	struct TGeometry
	{
		ALLOCATOR_BASED;
		TGeometry(bento::IAllocator& allocator)
		: vertices(allocator)
		, indices(allocator)
		{
			bento::SetIdentity(transform);
		}

		uint32_t numTriangles() 
		{
			return indices.size() / 3;
		}
		
		bento::Vector<bento::Vector3> vertices;
		bento::Vector<bento::IVector3> indices;
		bento::Matrix4 transform;
	};
}