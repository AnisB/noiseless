#ifndef RENDERER_H
#define RENDERER_H

#include "core/gpu_api.h"
#include "geometry/mesh.h"

namespace noiseless
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void init();
		void shutdown();

		void render_frame();
	private:
		GPUContext _gpu_context;
		GPUBuffer _frame_buffer;
	};
}

#endif // RENDERER_H