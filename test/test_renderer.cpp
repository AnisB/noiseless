#include <render/renderer.h>

int main()
{
	noiseless::Renderer renderer;
	renderer.init();
	renderer.render_frame();
	renderer.shutdown();
	return 0;
}