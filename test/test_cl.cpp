// Library includes
#include "core/gpu_api.h"
#include "base/log.h"
#include "base/security.h"

int main()
{
	// Define a number of element to process
	unsigned int num_elements = 10000000;

	// Create a gpu context
	noiseless::GPUContext context = noiseless::create_gpu_context();

	// Create init values
	std::vector<float> input_values(num_elements);
	for (uint64_t data_idx = 0; data_idx < num_elements; ++data_idx)
	{
		input_values[data_idx] = (float)data_idx;
	}
	// Create and write init value buffer
	noiseless::GPUBuffer input_buffer = noiseless::create_buffer(context, num_elements * sizeof(float), noiseless::GPUBufferInfo::READ_ONLY);
	noiseless::write_buffer(context, input_buffer, (unsigned char*)&input_values[0]);

	// Create an output buffer
	noiseless::GPUBuffer output_buffer = noiseless::create_buffer(context, num_elements * sizeof(float), noiseless::GPUBufferInfo::WRITE_ONLY);

	// Create a program and a kernel
	noiseless::GPUProgram program = noiseless::create_program(context, "kernel/square.kl");
	noiseless::GPUKernel kernel = noiseless::create_kernel(program, "process");

	// Sent the arguments
	noiseless::kernel_argument(kernel, 0, input_buffer);
	noiseless::kernel_argument(kernel, 1, output_buffer);
	noiseless::kernel_argument(kernel, 2, sizeof(unsigned int), (unsigned char*)&num_elements);

	// Launch the kernel
	noiseless::launch_kernel(context, kernel, num_elements);

	// Wait for the kernel to finish
	noiseless::wait_command_queue(context);

	// Read the output values
	std::vector<float> output_values(num_elements);
	noiseless::read_buffer(context, output_buffer, (unsigned char*)&output_values[0]);

	// Check the results
	for (uint64_t element_idx = 0; element_idx < num_elements; ++element_idx)
	{
		ASSERT((input_values[element_idx] * input_values[element_idx]) == output_values[element_idx] );
	}

	// Release all the gpu resources
	noiseless::destroy_buffer(output_buffer);
	noiseless::destroy_buffer(input_buffer);
	noiseless::destroy_kernel(kernel);
	noiseless::destroy_program(program);
	noiseless::destroy_gpu_context(context);
	return 0;
}