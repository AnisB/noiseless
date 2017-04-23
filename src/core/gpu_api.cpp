// Library includes
#include "gpu_api.h"

// External includes
#include <CL/cl.h>
#include <fstream>

namespace noiseless
{
	struct OpenCLContext
	{
		cl_device_id device_id;
		cl_context context;
		cl_command_queue commands;
		size_t global_dim;
	};

	struct OpenCLKernel
	{
		cl_program program;
	};

	GPUContext create_gpu_context()
	{
		// Instanciate a new cl context
		OpenCLContext* new_context = new OpenCLContext();

		// Declare the error holder
		cl_int error_flag;

		// Fetch the id of the first available device
		error_flag = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 1, &new_context->device_id, NULL);
	    if (error_flag != CL_SUCCESS)
	    {
	        delete new_context;
	        return 0;
	    }

	    // Create an opencl context
		new_context->context = clCreateContext(0, 1, &new_context->device_id, NULL, NULL, &error_flag);
	  	if (!new_context->context)
	    {
       		delete new_context;
	        return 0;
	    }

	    // Create a command queue
		new_context->commands = clCreateCommandQueueWithProperties(new_context->context, new_context->device_id, 0, &error_flag);
	    if (!new_context->commands)
	    {
	    	clReleaseContext(new_context->context);
       		delete new_context;
	        return 0;
	    }

	    // Fetch the device info
	    clGetDeviceInfo(new_context->device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), (void*)&new_context->global_dim, NULL);
	    
	    // All done lets go fella
	    return (uint64_t)new_context;
	}

	void destroy_gpu_context(GPUContext context)
	{
		OpenCLContext* target_context = (OpenCLContext*) context;
		clReleaseCommandQueue(target_context->commands);
		clReleaseContext(target_context->context);
       	delete target_context;
	}

	GPUKernel create_kernel(GPUContext context, const STRING_TYPE& source_file)
	{
		// Fetch the context
		OpenCLContext* target_context = (OpenCLContext*)context;

		// Read the source file
		std::ifstream in(source_file);
		std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		const char* kernelSource = contents.c_str();

		// Create a program
		OpenCLKernel* kernel = new OpenCLKernel();
		cl_int error_flag;
		kernel->program = clCreateProgramWithSource(target_context->context, 1, &kernelSource, NULL, &error_flag);
		if (error_flag || !kernel->program)
		{
			delete kernel;
			return 0;
		}
		error_flag = clBuildProgram(kernel->program, 0, NULL, NULL, NULL, NULL);
		if (error_flag != CL_SUCCESS)
		{
			size_t len;
			char buffer[2048];
			clGetProgramBuildInfo(kernel->program, target_context->device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
			clReleaseProgram(kernel->program);
			delete kernel;
			return 0;
		}
		return (uint64_t)kernel;
	}

	void destroy_kernel(GPUKernel gpu_kernel)
	{
		OpenCLKernel* kernel = (OpenCLKernel*)gpu_kernel;
		clReleaseProgram(kernel->program);
		delete kernel;
	}
}