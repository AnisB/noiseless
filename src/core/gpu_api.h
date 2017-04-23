#ifndef GPU_API_H
#define GPU_API_H

// Library includes
#include "base/string_helper.h"

// External includes
#include <stdint.h>

namespace noiseless
{
	// Useful types
	typedef uint64_t GPUContext;
	typedef uint64_t GPUKernel;

	GPUContext create_gpu_context();
	void destroy_gpu_context(GPUContext context);
	GPUKernel create_kernel(GPUContext context, const STRING_TYPE& source_file);
	void destroy_kernel(GPUKernel kernel);
}

#endif // GPU_API_H