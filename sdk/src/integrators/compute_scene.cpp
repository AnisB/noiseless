// Internal include
#include "integrators/compute_scene.h"
#include "gpu_backend/compute_api.h"

namespace noiseless
{
	void create_compute_scene(ComputeContext computeContext, const TScene& scene, TComputeScene& outputComputeScene)
	{
		// Destroy the vertex buffer if not null
		if(outputComputeScene.vertices != 0)
		{
			destroy_buffer(outputComputeScene.vertices);
			outputComputeScene.vertices = 0;
		}

		// Destroy the index buffer if not null
		if(outputComputeScene.indices != 0)
		{
			destroy_buffer(outputComputeScene.indices);
			outputComputeScene.indices = 0;
		}

		// Fetch the mesh count
		uint32_t numMeshes = scene.num_meshes();

		// First of all compute the full size of the buffer
		uint32_t totalVerticeCount = 0;
		uint32_t totalIndexCount = 0;
		for(uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const TMesh& currentMesh = scene.mesh(meshIndex);
			totalVerticeCount += currentMesh.vertices.size();
			totalIndexCount += currentMesh.indices.size();
		}

		// Let's now allocate the combined buffers, we have to allocate size 4 data because size 3 does not exist
		bento::Vector<bento::Vector4> combinedVerticeArray(*bento::common_allocator(), totalVerticeCount);
		bento::Vector<bento::IVector4> combinedIndicesArray(*bento::common_allocator(), totalIndexCount);

		// Let's fill the combined buffer
		uint32_t vertexIterator = 0;
		uint32_t indexIterator = 0;
		for(uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const TMesh& currentMesh = scene.mesh(meshIndex);
			uint32_t numVertices = currentMesh.vertices.size();
			uint32_t numIndices = currentMesh.indices.size();

			// The vertex buffers does not require any specific processing except for the copy and the resize
			for (uint32_t vertIdx = 0; vertIdx < numVertices; ++vertIdx)
			{
				const bento::Vector3& currentVert = currentMesh.vertices[vertIdx];
				combinedVerticeArray[vertexIterator + vertIdx] = { currentVert.x, currentVert.y, currentVert.z, 0.0f};
			}

			// We need to copy the indices and shifting with the iterator value
			for(uint32_t faceIdx = 0; faceIdx < numIndices; ++faceIdx)
			{
				const bento::IVector3& faceIndices = currentMesh.indices[faceIdx];
				combinedIndicesArray[indexIterator + faceIdx] = {faceIndices.x + vertexIterator, faceIndices.y + vertexIterator, faceIndices.z + vertexIterator, 0};
			}

			vertexIterator += numVertices;
			indexIterator += numVertices;
		}

		// Let's now create the compute buffers
		outputComputeScene.vertices = create_buffer(computeContext, totalVerticeCount * sizeof(bento::Vector3), ComputeBufferType::READ_ONLY);
		write_buffer(computeContext, outputComputeScene.vertices, (unsigned char*)(combinedVerticeArray.begin()));
		outputComputeScene.indices = create_buffer(computeContext, totalIndexCount * sizeof(bento::IVector3), ComputeBufferType::READ_ONLY);
		write_buffer(computeContext, outputComputeScene.indices, (unsigned char*)(combinedIndicesArray.begin()));
	}

	void destroy_compute_scene(TComputeScene& targetComputeScene)
	{
		// Destroy the vertex buffer if not null
		if (targetComputeScene.vertices != 0)
		{
			destroy_buffer(targetComputeScene.vertices);
			targetComputeScene.vertices = 0;
		}

		// Destroy the index buffer if not null
		if (targetComputeScene.indices != 0)
		{
			destroy_buffer(targetComputeScene.indices);
			targetComputeScene.indices = 0;
		}
	}
}