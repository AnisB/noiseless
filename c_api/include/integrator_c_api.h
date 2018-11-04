#pragma once

#include "types_c_api.h"

extern "C"
{
	NL_EXPORT NLIntegratorObject* nl_create_integrator(NLAllocatorObject* allocator);
	NL_EXPORT void nl_destroy_integrator(NLIntegratorObject* integrator);

	NL_EXPORT void nl_integrator_initialize_resources(NLIntegratorObject* integrator, NLADBObject* assetDatabase);
	NL_EXPORT void nl_integrator_release_resources(NLIntegratorObject* integrator);

	NL_EXPORT void nl_integrator_setup_scene(NLIntegratorObject* integrator, NLSceneObject* scene);
	NL_EXPORT void nl_integrator_clear_scene(NLIntegratorObject* integrator);

	NL_EXPORT void nl_integrator_set_resolution(NLIntegratorObject* integrator, uint32_t width, uint32_t height);

	NL_EXPORT void nl_integrator_render(NLIntegratorObject* integrator, float* viewMatrix, float fov, float nearP, float farP);
}