// Library includes
#include "core/gpu_api.h"

int main()
{
	noiseless::GPUContext context = noiseless::create_gpu_context();
	noiseless::destroy_gpu_context(context);
	return 0;
}