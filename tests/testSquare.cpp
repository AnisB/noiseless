// Library includes
#include "bento_base/log.h"
#include "bento_base/security.h"

// bento includes
#include <bento_base/log.h>
#include <bento_collection/vector.h>
#include <bento_collection/dynamic_string.h>
#include <bento_base/security.h>
#include <bento_compute/compute_api.h>

const char* squareKernel = "\
__kernel void process(__global float* A, __global float* B, const uint num_elements)\
{\
	int id = get_global_id(0);\
	int num_workers = get_global_size(0);\
\
	for (int i = id; i < num_elements; i += num_workers)\
	{\
		B[i] = A[i] * A[i];\
	}\
}";

int main()
{
	// Allocator used for this program
	bento::IAllocator& current_alloc = *bento::common_allocator();

	// Define a number of element to process
	const uint32_t num_elements = 10000000;

	// Create a compute context
	bento::ComputeContext context = bento::create_compute_context(current_alloc);

	// Create a command list
	bento::ComputeCommandList commandList = bento::create_command_list(context, current_alloc);

	// Create init values
	bento::Vector<float> input_values(current_alloc, num_elements);
	for (uint32_t data_idx = 0; data_idx < num_elements; ++data_idx)
	{
		input_values[data_idx] = (float)data_idx;
	}
	// Create and write init value buffer
	bento::ComputeBuffer input_buffer = bento::create_buffer(context, num_elements * sizeof(float), bento::ComputeBufferType::READ_ONLY, current_alloc);
	bento::write_buffer(commandList, input_buffer, (unsigned char*)&input_values[0]);

	// Create an output buffer
	bento::ComputeBuffer output_buffer = bento::create_buffer(context, num_elements * sizeof(float), bento::ComputeBufferType::WRITE_ONLY, current_alloc);

	// Create a program and a kernel
	bento::ComputeProgram program = bento::create_program_source(context, squareKernel);
	bento::ComputeKernel kernel = bento::create_kernel(program, "process");

	// Sent the arguments
	bento::kernel_argument(kernel, 0, input_buffer);
	bento::kernel_argument(kernel, 1, output_buffer);
	bento::kernel_argument(kernel, 2, sizeof(unsigned int), (unsigned char*)&num_elements);

	// Launch the kernel
	bento::dispatch_kernel_1D(commandList, kernel, num_elements);

	// Wait for the kernel to finish
	bento::flush_command_list(commandList);

	// Read the output values
	bento::Vector<float> output_values(*bento::common_allocator(), num_elements);
	bento::read_buffer(commandList, output_buffer, (unsigned char*)&output_values[0]);

	// Check the results
	for (uint32_t element_idx = 0; element_idx < num_elements; ++element_idx)
	{
		assert((input_values[element_idx] * input_values[element_idx]) == output_values[element_idx]);
	}
	bento::default_logger()->log(bento::LogLevel::info, "TEST SQUARE", "Passed");

	// Release all the compute resources
	bento::destroy_buffer(output_buffer);
	bento::destroy_buffer(input_buffer);
	bento::destroy_kernel(kernel);
	bento::destroy_program(program);
	bento::destroy_command_list(commandList);
	bento::destroy_compute_context(context);
	return 0;
}