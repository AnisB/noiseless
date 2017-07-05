// Library includes
#include "renderer.h"
#include "core/gpu_api.h"
#include "base/security.h"

namespace noiseless
{
	Renderer::Renderer()
	: _gpu_context(0)
	, _frame_buffer(0)
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::init()
	{
		// Create the gpu context that will allow us to procceed with the rendering
		_gpu_context = noiseless::create_gpu_context();

		// Create the frame buffer that will contain all the memory space needed for rendering
		_frame_buffer = create_buffer(_gpu_context, 1280 * 720, GPUBufferInfo::READ_WRITE);

		// Create a program from the renderer
		_program = noiseless::create_program(_gpu_context, "kernel/intersect.kl");

		// Create a kernel for the intersection code
		_intersection_kernel = noiseless::create_kernel(_program, "process");
	}
	
	void Renderer::shutdown()
	{
		destroy_kernel(_intersection_kernel);
		destroy_program(_program);
		// Destroy the gpu context associated with this renderer
		ASSERT_MSG(_gpu_context != 0, "Renderer::shutdown; shutdown can't be called if init was not called before");
		noiseless::destroy_gpu_context(_gpu_context);
	}

	void Renderer::render_frame()
	{

	}
}