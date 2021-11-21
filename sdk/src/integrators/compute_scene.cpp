// bento includes
#include <bento_rt/bvh.h>
#include <bento_base/security.h>

// Internal include
#include "integrators/compute_scene.h"

namespace noiseless
{
	struct PackedScene
	{
		ALLOCATOR_BASED;
		PackedScene(bento::IAllocator& allocator)
		: verticesArray(allocator)
		, indicesArray(allocator)
		, additionalData(allocator) 
		{
		}
		bento::Vector<bento::Vector3> verticesArray;
		bento::Vector<bento::IVector3> indicesArray;
		bento::Vector<bento::IVector4> additionalData;
	};

	namespace packed_scene
	{
		uint32_t num_primitives(const bento::bvh::Container* container)
		{
			// Cast and validate the packed scene
			PackedScene* ps = (PackedScene*)container;
			assert(ps != nullptr);

			// return the primitive count
			return ps->indicesArray.size() / 3;
		}

		bento::Box3 box(const bento::bvh::Container* container, uint32_t primitive_idx)
		{
			// Cast and validate the packed scene
			PackedScene* ps = (PackedScene*)container;
			assert(ps != nullptr);

			// Prepare the box
			bento::Box3 box;
			bento::box3::reset(box);

			// Get the indices
			const bento::IVector3& indices = ps->indicesArray[primitive_idx];
			bento::box3::include_point(box, ps->verticesArray[indices.x]);
			bento::box3::include_point(box, ps->verticesArray[indices.y]);
			bento::box3::include_point(box, ps->verticesArray[indices.z]);
			return box;
		}
	}

	void release_compute_buffers(TComputeScene& computeScene)
	{
		// Destroy the vertex buffer if not null
		if (computeScene.vertices != 0)
		{
			bento::destroy_buffer(computeScene.vertices);
			computeScene.vertices = 0;
		}

		// Destroy the index buffer if not null
		if (computeScene.indices != 0)
		{
			bento::destroy_buffer(computeScene.indices);
			computeScene.indices = 0;
		}
	}

	void evaluate_indice_and_vertex_count(const TScene& scene, uint32_t& totalIndexCount, uint32_t& totalVerticeCount)
	{
		// Fetch the mesh count
		uint32_t numMeshes = scene.num_meshes();

		// First of all compute the full size of the buffer
		totalVerticeCount = 0;
		totalIndexCount = 0;
		for (uint32_t geometryIndex = 0; geometryIndex < numMeshes; ++geometryIndex)
		{
			const TGeometry	& currentGeometry = scene.geometry(geometryIndex);
			totalVerticeCount += currentGeometry.vertices.size();
			totalIndexCount += currentGeometry.indices.size();
		}
	}

	void combine_indices_and_vertices_array(const TScene& scene, bento::Vector<bento::Vector3>& verticesArray, bento::Vector<bento::IVector3>& indicesArray)
	{
		// Clear the buffers
		verticesArray.clear();
		indicesArray.clear();

		// Fetch the mesh count
		uint32_t numMeshes = scene.num_meshes();

		// Let's fill the combined buffer
		uint32_t vertexIterator = 0;
		uint32_t indexIterator = 0;
		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const TGeometry& currentGeometry = scene.geometry(meshIndex);
			uint32_t numVertices = currentGeometry.vertices.size();
			uint32_t numIndices = currentGeometry.indices.size();

			// The vertex buffers does not require any specific processing except for the copy and the resize
			for (uint32_t vertIdx = 0; vertIdx < numVertices; ++vertIdx)
			{
				const bento::Vector3& currentVert = currentGeometry.vertices[vertIdx];
				verticesArray[vertexIterator + vertIdx] = { currentVert.x, currentVert.y, currentVert.z };
			}

			// We need to copy the indices and shifting with the iterator value
			for (uint32_t faceIdx = 0; faceIdx < numIndices; ++faceIdx)
			{
				const bento::IVector3& faceIndices = currentGeometry.indices[faceIdx];
				indicesArray[indexIterator + faceIdx] = { faceIndices.x + vertexIterator, faceIndices.y + vertexIterator, faceIndices.z + vertexIterator };
			}

			vertexIterator += numVertices;
			indexIterator += numVertices;
		}
	}

	void create_compute_scene(bento::ComputeContext computeContext, const TScene& scene, TComputeScene& outputComputeScene, bento::IAllocator& allocator)
	{
		// Make sure to release the compute buffers if they have been previously allocated
		release_compute_buffers(outputComputeScene);

		// First of all compute the full size of the buffer
		uint32_t totalVerticeCount = 0;
		uint32_t totalIndexCount = 0;
		evaluate_indice_and_vertex_count(scene, totalIndexCount, totalVerticeCount);

		// Let's now allocate the combined buffers
		PackedScene packedScene(allocator);
		packedScene.verticesArray.resize(totalVerticeCount);
		packedScene.indicesArray.resize(totalIndexCount);
		combine_indices_and_vertices_array(scene, packedScene.verticesArray, packedScene.indicesArray);

		// Let's now create the compute buffers on GPU side and push the combined array
		outputComputeScene.vertices = bento::create_buffer(computeContext, totalVerticeCount * sizeof(bento::Vector3), bento::ComputeBufferType::READ_ONLY, allocator);
		bento::write_buffer(computeContext, outputComputeScene.vertices, (unsigned char*)(packedScene.verticesArray.begin()));
		outputComputeScene.indices = bento::create_buffer(computeContext, totalIndexCount * sizeof(bento::IVector3), bento::ComputeBufferType::READ_ONLY, allocator);
		bento::write_buffer(computeContext, outputComputeScene.indices, (unsigned char*)(packedScene.indicesArray.begin()));
		
		// Build the accessor to get our scene
		bento::bvh::Accessor sceneAccessor;
		sceneAccessor.container = (bento::bvh::Container*)&packedScene;
		sceneAccessor.num_primitives = packed_scene::num_primitives;
		sceneAccessor.box = packed_scene::box;
		sceneAccessor.intersect = nullptr; //  NOT NEEDED FOR NOW

		// Now we need to build the BVH that we will also need to push on the GPU
		bento::Bvh bvh(allocator);
		bento::bvh::build(bvh, sceneAccessor);

		// Let's now create the compute buffers on GPU side and push the BVH arrays
		outputComputeScene.bvhNodes = bento::create_buffer(computeContext, bvh.nodes.size() * sizeof(bento::BvhNode), bento::ComputeBufferType::READ_ONLY, allocator);
		bento::write_buffer(computeContext, outputComputeScene.vertices, (unsigned char*)(bvh.nodes.begin()));
		outputComputeScene.bvhPrimitives = bento::create_buffer(computeContext, bvh.primitives.size() * sizeof(bento::BvhPrimitive), bento::ComputeBufferType::READ_ONLY, allocator);
		bento::write_buffer(computeContext, outputComputeScene.bvhPrimitives, (unsigned char*)(bvh.primitives.begin()));
	}

	void destroy_compute_scene(TComputeScene& targetComputeScene)
	{
		// Make sure to release the compute buffers if they have been previously allocated
		release_compute_buffers(targetComputeScene);
	}
}