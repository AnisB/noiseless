#pragma once

#include "types_c_api.h"

extern "C"
{
	// Function to create a new noiseless scene
	NL_EXPORT NLSceneObject* nl_create_scene(NLAllocatorObject* allocator);

	// Function to set the number of meshes of a given scene
	NL_EXPORT void nl_scene_set_num_meshes(NLSceneObject* scene, uint32_t numMeshes);

	// Function to set the data of a given mesh
	NL_EXPORT void nl_scene_set_mesh_data(NLSceneObject* scene, uint32_t meshIndex, float* vertexArray, uint32_t numVerts, int* indicesArray, uint32_t numIndices);

	// Function to destroy a noiseless scene
	NL_EXPORT void nl_destroy_scene(NLSceneObject* scene);
}