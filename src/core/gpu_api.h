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
	typedef uint64_t GPUProgram;
	typedef uint64_t GPUKernel;
	typedef uint64_t GPUBuffer;

	GPUContext create_gpu_context();
	void destroy_gpu_context(GPUContext context);

	GPUProgram create_program(GPUContext context, const STRING_TYPE& source_file);
	void destroy_program(GPUProgram program);

	GPUKernel create_kernel(GPUProgram program, const STRING_TYPE& kernel_name);
	bool launch_kernel(GPUContext context, GPUKernel kernel, uint64_t job_size);
	void wait_command_queue(GPUContext context);
	void destroy_kernel(GPUKernel kernel);

	namespace GPUBufferInfo
	{
		enum Type
		{
			READ_ONLY = 4,
			WRITE_ONLY= 2,
			READ_WRITE = 1
		};
	}

	GPUBuffer create_buffer(GPUContext context, uint64_t buffer_size, GPUBufferInfo::Type buffer_type);
	bool read_buffer(GPUContext context, GPUBuffer buffer, unsigned char* output_data);
	bool write_buffer(GPUContext context, GPUBuffer buffer, unsigned char* intput_data);
	void destroy_buffer(GPUBuffer buffer);

	bool kernel_argument(GPUKernel kernel, int idx, GPUBuffer buffer);
	bool kernel_argument(GPUKernel kernel, int idx, uint64_t size, unsigned char* raw_data);
}

#endif // GPU_API_H