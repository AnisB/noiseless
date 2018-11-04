#pragma once

// SDK includes
#include "compute_types.h"

// External includes
#include <stdint.h>

namespace noiseless
{
	ComputeContext create_compute_context();
	void destroy_compute_context(ComputeContext context);

	ComputeProgram create_program_source(ComputeContext context, const char* programSource);
	void destroy_program(ComputeProgram program);

	ComputeKernel create_kernel(ComputeProgram program, const char* kernel_name);
	bool launch_kernel_1D(ComputeContext context, ComputeKernel kernel, uint64_t job_size);
	bool launch_kernel_2D(ComputeContext context, ComputeKernel kernel, uint64_t job_size_0, uint64_t job_size_1);
	void wait_command_queue(ComputeContext context);
	void destroy_kernel(ComputeKernel kernel);

	ComputeBuffer create_buffer(ComputeContext context, uint64_t buffer_size, ComputeBufferType::Type buffer_type);
	bool read_buffer(ComputeContext context, ComputeBuffer buffer, unsigned char* output_data);
	bool write_buffer(ComputeContext context, ComputeBuffer buffer, unsigned char* intput_data);
	void destroy_buffer(ComputeBuffer buffer);

	bool kernel_argument(ComputeBuffer kernel, int idx, ComputeBuffer buffer);
	bool kernel_argument(ComputeBuffer kernel, int idx, uint32_t size, unsigned char* raw_data);
}