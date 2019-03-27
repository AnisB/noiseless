#pragma once

// External includes
#include <stdint.h>

namespace noiseless
{
	// Opaque types of the Backend API
	typedef uint64_t ComputeContext;
	typedef uint64_t ComputeCommandList;
	typedef uint64_t ComputeProgram;
	typedef uint64_t ComputeKernel;
	typedef uint64_t ComputeBuffer;


	namespace ComputeBufferType
	{
		enum Type
		{
			READ_ONLY = 4,
			WRITE_ONLY = 2,
			READ_WRITE = 1
		};
	}
}