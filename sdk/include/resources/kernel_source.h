#pragma once

// Bento includes
#include <bento_collection/dynamic_string.h>

namespace noiseless
{
	struct TKernelSource
	{
		TKernelSource(bento::IAllocator& alloc)
		: data(alloc)
		{
		}
		bento::Vector<char> data;
	};
}

namespace bento
{
	void pack_type(bento::Vector<char>& buffer, const noiseless::TKernelSource& shader_source);
	bool unpack_type(const char*& stream, noiseless::TKernelSource& shader_source);
}
