#ifndef MESH_H
#define MESH_H

// Library include
#include "math/types.h"
#include "containers/vector.h"
#include "core/gpu_api.h"

// External include
#include <vector>

namespace noiseless
{
	struct Mesh
	{
		std::vector<Vector3> verts;
		std::vector<Vector3> normals;
		std::vector<IVector3> faces;
	};

	struct GPUMesh
	{
		GPUBuffer verts;
		uint32_t num_verts;
		GPUBuffer normals;
		uint32_t num_normals;
		GPUBuffer faces;
		uint32_t num_faces;
	};

	void build_gpu_mesh(GPUContext context, const Mesh& mesh, GPUMesh& output);
	void destroy_gpu_mesh(GPUMesh& target_mesh);
}

#endif // MESH_H