// C API includes
#include "integrator_c_api.h"

// SDK includes
#include <integrators/integrator.h>

NLIntegratorObject* nl_create_integrator(NLAllocatorObject* allocator)
{
	bento::IAllocator* allocPtr = (bento::IAllocator*)allocator;
	noiseless::TIntegrator* integratorPtr = bento::make_new<noiseless::TIntegrator>(*allocPtr, *allocPtr);
	return (NLIntegratorObject*)integratorPtr;
}

void nl_destroy_integrator(NLIntegratorObject* integrator)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	bento::make_delete<noiseless::TIntegrator>(integratorPtr->allocator, integratorPtr);
}

void nl_integrator_initialize_resources(NLIntegratorObject* integrator, NLADBObject* assetDatabase)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	bento::TAssetDatabase* adbPtr = (bento::TAssetDatabase*)(assetDatabase);
	integratorPtr->init_resources(*adbPtr);
}

void nl_integrator_release_resources(NLIntegratorObject* integrator)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	integratorPtr->release_resources();
}

void nl_integrator_setup_scene(NLIntegratorObject* integrator, NLSceneObject* scene)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	noiseless::TScene* scenePtr = (noiseless::TScene*)(scene);
	integratorPtr->setup_scene(*scenePtr);
}

void nl_integrator_clear_scene(NLIntegratorObject* integrator)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	integratorPtr->clear_scene();
}

void nl_integrator_set_resolution(NLIntegratorObject* integrator, uint32_t width, uint32_t height)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	integratorPtr->set_resolution(width, height);
}

void nl_integrator_render(NLIntegratorObject* integrator, float* viewMatrix, float fov, float nearP, float farP)
{
	noiseless::TIntegrator* integratorPtr = (noiseless::TIntegrator*)(integrator);
	noiseless::TCamera camera;
	memcpy(camera.view.m, viewMatrix, 16 * sizeof(float));
	camera.fov = fov;
	camera.nearPlane = nearP;
	camera.farPlane = farP;
	integratorPtr->render(camera);
}