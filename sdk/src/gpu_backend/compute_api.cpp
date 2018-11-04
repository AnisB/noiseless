// Library includes
#include "gpu_backend/compute_api.h"
#include "bento_base/security.h"

// External includes
#include <CL/cl.h>
#include <fstream>
#include <algorithm>
#include <vector>

namespace noiseless
{
	struct OpenCLContext
	{
		cl_device_id device_id;
		cl_context context;
		cl_command_queue commands;
		size_t global_dim;
	};

	struct OpenCLBuffer
	{
		cl_mem buffer;
		uint64_t size;
		ComputeBufferType::Type type;
	};

	uint64_t evaluate_platforms(std::vector<cl_platform_id>& platform_ids)
	{
		uint64_t final_platform = 0;
		size_t global_dim = 0;
		uint64_t num_platforms = platform_ids.size();
		for (uint64_t plat_idx = 0; plat_idx < num_platforms; ++plat_idx)
		{
			cl_device_id device_id;
			clGetDeviceIDs(platform_ids[plat_idx], CL_DEVICE_TYPE_ALL, 1, &device_id, NULL);
			size_t candidate_dim;
			clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &candidate_dim, NULL);
			if (candidate_dim > global_dim)
			{
				final_platform = plat_idx;
				global_dim = candidate_dim;
			}
		}
		return final_platform;
	}

	ComputeContext create_compute_context()
	{
		// Instanciate a new cl context
		OpenCLContext* new_context = new OpenCLContext();

		// Declare the error holder
		cl_int error_flag;

		// Get the number of platforms
		cl_uint platformIdCount = 0;
		clGetPlatformIDs(0, nullptr, &platformIdCount);

		// Get all the platforms
		std::vector<cl_platform_id> platformIds(platformIdCount);
		clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);

		uint64_t best_platform = evaluate_platforms(platformIds);

		// Fetch the id of the first available device
		error_flag = clGetDeviceIDs(platformIds[best_platform], CL_DEVICE_TYPE_ALL, 1, &new_context->device_id, NULL);
	    if (error_flag != CL_SUCCESS)
	    {
	        delete new_context;
			assert_fail_msg("Can't fetch devices");
			return 0;
	    }

	    // Create an opencl context
		new_context->context = clCreateContext(0, 1, &new_context->device_id, NULL, NULL, &error_flag);
	  	if (!new_context->context)
	    {
       		delete new_context;
			assert_fail_msg("Can't create context");
	        return 0;
	    }

	    // Create a command queue
		new_context->commands = clCreateCommandQueueWithProperties(new_context->context, new_context->device_id, 0, &error_flag);
	    if (!new_context->commands)
	    {
	    	clReleaseContext(new_context->context);
       		delete new_context;
			assert_fail_msg("Can't create command queue");
	        return 0;
	    }

	    // Fetch the device info
	    clGetDeviceInfo(new_context->device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), (void*)&new_context->global_dim, NULL);
	    
	    // All done lets go fella
	    return (ComputeContext)new_context;
	}

	void destroy_compute_context(ComputeContext context)
	{
		OpenCLContext* target_context = (OpenCLContext*) context;
		clReleaseCommandQueue(target_context->commands);
		clReleaseContext(target_context->context);
       	delete target_context;
	}

	ComputeProgram create_program_source(ComputeContext context, const char* programSource)
	{
		// Fetch the context
		OpenCLContext* target_context = (OpenCLContext*)context;

		// Create a program
		cl_program program;
		cl_int error_flag;
		program = clCreateProgramWithSource(target_context->context, 1, &programSource, NULL, &error_flag);
		if (error_flag || !program)
		{
			assert_fail_msg("Can't create program with source");
			return 0;
		}
		error_flag = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
		if (error_flag != CL_SUCCESS)
		{
			size_t len;
			char buffer[2048];
			clGetProgramBuildInfo(program, target_context->device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
			clReleaseProgram(program);
			assert_fail_msg(buffer);
			return 0;
		}
		return (ComputeProgram)program;
	}

	void destroy_program(ComputeProgram gpu_program)
	{
		clReleaseProgram((cl_program)gpu_program);
	}

	ComputeKernel create_kernel(ComputeProgram program, const char* kernel_name)
	{
		cl_int error_flag;
		cl_kernel kernel = clCreateKernel((cl_program)program, kernel_name, &error_flag);
		if (!kernel || error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't create kernel");
			return 0;
		}
		return (ComputeKernel)kernel;
	}

	bool launch_kernel_1D(ComputeContext context, ComputeKernel kernel, size_t job_size)
	{
		OpenCLContext* target_context = (OpenCLContext*)context;

		size_t maxDimension = std::min(job_size, target_context->global_dim);
		cl_int error_flag = clEnqueueNDRangeKernel(target_context->commands, (cl_kernel)kernel, 1, nullptr, &maxDimension, nullptr, 0, NULL, NULL);
		if (error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't execute kernel");
			return false;
		}
		return true;
	}

	bool launch_kernel_2D(ComputeContext context, ComputeKernel kernel, uint64_t job_size_0, uint64_t job_size_1)
	{
		OpenCLContext* target_context = (OpenCLContext*)context;

		size_t global_dim_0 = std::min(job_size_0, target_context->global_dim);
		size_t global_dim_1 = std::min(job_size_1, target_context->global_dim);
		size_t global_dim[2] = { global_dim_0, global_dim_1 };
		cl_int error_flag = clEnqueueNDRangeKernel(target_context->commands, (cl_kernel)kernel, 2, nullptr, global_dim, nullptr, 0, NULL, NULL);
		if (error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't execute kernel");
			return false;
		}
		return true;
	}

	void wait_command_queue(ComputeContext context)
	{
		OpenCLContext* target_context = (OpenCLContext*)context;
		clFinish(target_context->commands);
	}

	void destroy_kernel(ComputeKernel kernel)
	{
		clReleaseKernel((cl_kernel)kernel);
	}

	ComputeBuffer create_buffer(ComputeContext context, uint64_t buffer_size, ComputeBufferType::Type buffer_type)
	{
		OpenCLContext* target_context = (OpenCLContext*)context;
		OpenCLBuffer* buffer = new OpenCLBuffer();
		buffer->size = buffer_size;
		buffer->type = buffer_type;
		cl_int error_flag;
		buffer->buffer = clCreateBuffer(target_context->context, buffer_type, buffer_size, NULL, &error_flag);
		// Copy data to input buffer
		if (error_flag != CL_SUCCESS)
		{
			delete buffer;
			assert_fail_msg("Can't create buffer");
			return 0;
		}
		return (ComputeBuffer)buffer;
	}

	bool read_buffer(ComputeContext context, ComputeBuffer buffer, unsigned char* output_data)
	{
		OpenCLContext* target_context = (OpenCLContext*)context;
		OpenCLBuffer* target_buffer = (OpenCLBuffer*)buffer;
		cl_int error_flag = clEnqueueReadBuffer(target_context->commands, target_buffer->buffer, CL_TRUE, 0, target_buffer->size, output_data, 0, NULL, NULL);
		if (error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't read buffer");
			return false;
		}
		return true;
	}

	bool write_buffer(ComputeContext context, ComputeBuffer buffer, unsigned char* intput_data)
	{
		OpenCLContext* target_context = (OpenCLContext*)context;
		OpenCLBuffer* target_buffer = (OpenCLBuffer*)buffer;
		cl_int error_flag = clEnqueueWriteBuffer(target_context->commands, target_buffer->buffer, CL_TRUE, 0, target_buffer->size, intput_data, 0, NULL, NULL);
		if (error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't write buffer");
			return false;
		}
		return true;
	}

	void destroy_buffer(ComputeBuffer buffer)
	{
		OpenCLBuffer* current_buffer = (OpenCLBuffer*)buffer;
		clReleaseMemObject((cl_mem)current_buffer->buffer);
		delete current_buffer;
	}

	bool kernel_argument(ComputeContext kernel, int idx, uint32_t size, unsigned char* raw_data)
	{
		cl_int error_flag = clSetKernelArg((cl_kernel)kernel, idx, size, raw_data);
		if (error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't set kernel argument");
			return false;
		}
		return true;
	}

	bool kernel_argument(ComputeKernel kernel, int idx, ComputeBuffer buffer)
	{
		OpenCLBuffer* current_buffer = (OpenCLBuffer*)buffer;
		cl_int error_flag = clSetKernelArg((cl_kernel)kernel, idx, sizeof(cl_mem), &(current_buffer->buffer));
		if (error_flag != CL_SUCCESS)
		{
			assert_fail_msg("Can't set kernel argument");
			return false;
		}
		return true;
	}
}