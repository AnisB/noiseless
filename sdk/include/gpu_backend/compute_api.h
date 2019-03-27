#pragma once

// SDK includes
#include "compute_types.h"

// External includes
#include <stdint.h>

namespace noiseless
{
	// Context creation and destruction
	ComputeContext create_compute_context();
	void destroy_compute_context(ComputeContext context);

	// Program create and destruction
	ComputeProgram create_program_source(ComputeContext context, const char* programSource);
	void destroy_program(ComputeProgram program);

	// Command list creation and destruction
	ComputeCommandList create_command_list(ComputeContext context);
	void destroy_command_list(ComputeCommandList computeCommandList);

	// Kernel creation and destruction
	ComputeKernel create_kernel(ComputeProgram program, const char* kernel_name);
	void destroy_kernel(ComputeKernel kernel);

	// Buffer creation
	ComputeBuffer create_buffer(ComputeContext context, uint64_t buffer_size, ComputeBufferType::Type buffer_type);
	void destroy_buffer(ComputeBuffer buffer);

	// Command enquing
	bool dispatch_kernel_1D(ComputeCommandList commandList, ComputeKernel kernel, uint64_t job_size);
	bool dipsatch_kernel_2D(ComputeCommandList commandList, ComputeKernel kernel, uint64_t job_size_0, uint64_t job_size_1);
	bool read_buffer(ComputeCommandList commandList, ComputeBuffer buffer, unsigned char* output_data);
	bool write_buffer(ComputeCommandList commandList, ComputeBuffer buffer, unsigned char* intput_data);

	// Flushing the command list
	void flush_command_list(ComputeCommandList commandList);

	// Setting parameters to kernels
	bool kernel_argument(ComputeBuffer kernel, int idx, ComputeBuffer buffer);
	bool kernel_argument(ComputeBuffer kernel, int idx, uint32_t size, unsigned char* raw_data);
}