// C API includes
#include "scene_c_api.h"

// SDK includes
#include <model/scene.h>

// Function to create a new noiseless scene
NLSceneObject* nl_create_scene(NLAllocatorObject* allocator)
{
	bento::IAllocator* allocPtr = (bento::IAllocator*)allocator;
	noiseless::TScene* scenePtr = bento::make_new<noiseless::TScene>(*allocPtr, *allocPtr);
	return (NLSceneObject*)scenePtr;
}

// Function to set the number of meshes of a given scene
void nl_scene_set_num_meshes(NLSceneObject* scene, uint32_t numMeshes)
{
	noiseless::TScene* scenePtr = (noiseless::TScene*)(scene);
	scenePtr->set_num_meshes(numMeshes);
}

// Function to set the data of a given mesh
void nl_scene_set_mesh_data(NLSceneObject* scene, uint32_t meshIndex, float* vertexArray, uint32_t numVerts, int* indicesArray, uint32_t numIndices)
{
	noiseless::TScene* scenePtr = (noiseless::TScene*)(scene);
	noiseless::TGeometry& targetGeometry = scenePtr->geometry(meshIndex);
	targetGeometry.vertices.resize(numVerts);
	targetGeometry.indices.resize(numIndices);
	memcpy(targetGeometry.vertices.begin(), vertexArray, sizeof(bento::Vector3) * numVerts);
	memcpy(targetGeometry.indices.begin(), indicesArray, sizeof(bento::IVector3) * numIndices);
}

// Function to destroy a noiseless scene
void nl_destroy_scene(NLSceneObject* scene)
{
	noiseless::TScene* scenePtr = (noiseless::TScene*)(scene);
	bento::make_delete<noiseless::TScene>(scenePtr->allocator, scenePtr);
}