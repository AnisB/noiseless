// Library includes
#include "gpu_backend/compute_api.h"
#include "bento_base/log.h"
#include "bento_base/security.h"

// Bento includes
#include <bento_base/log.h>
#include <bento_collection/vector.h>
#include <bento_collection/dynamic_string.h>
#include <bento_base/security.h>

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

	// Create a gpu context
	noiseless::ComputeContext context = noiseless::create_compute_context();

	// Create init values
	bento::Vector<float> input_values(*bento::common_allocator(), num_elements);
	for (uint32_t data_idx = 0; data_idx < num_elements; ++data_idx)
	{
		input_values[data_idx] = (float)data_idx;
	}
	// Create and write init value buffer
	noiseless::ComputeBuffer input_buffer = noiseless::create_buffer(context, num_elements * sizeof(float), noiseless::ComputeBufferType::READ_ONLY);
	noiseless::write_buffer(context, input_buffer, (unsigned char*)&input_values[0]);

	// Create an output buffer
	noiseless::ComputeBuffer output_buffer = noiseless::create_buffer(context, num_elements * sizeof(float), noiseless::ComputeBufferType::WRITE_ONLY);

	// Create a program and a kernel
	noiseless::ComputeProgram program = noiseless::create_program_source(context, squareKernel);
	noiseless::ComputeKernel kernel = noiseless::create_kernel(program, "process");

	// Sent the arguments
	noiseless::kernel_argument(kernel, 0, input_buffer);
	noiseless::kernel_argument(kernel, 1, output_buffer);
	noiseless::kernel_argument(kernel, 2, sizeof(unsigned int), (unsigned char*)&num_elements);

	// Launch the kernel
	noiseless::launch_kernel_1D(context, kernel, num_elements);

	// Wait for the kernel to finish
	noiseless::wait_command_queue(context);

	// Read the output values
	bento::Vector<float> output_values(*bento::common_allocator(), num_elements);
	noiseless::read_buffer(context, output_buffer, (unsigned char*)&output_values[0]);

	// Check the results
	for (uint32_t element_idx = 0; element_idx < num_elements; ++element_idx)
	{
		assert((input_values[element_idx] * input_values[element_idx]) == output_values[element_idx] );
	}

	// Release all the gpu resources
	noiseless::destroy_buffer(output_buffer);
	noiseless::destroy_buffer(input_buffer);
	noiseless::destroy_kernel(kernel);
	noiseless::destroy_program(program);
	noiseless::destroy_compute_context(context);
	return 0;
}