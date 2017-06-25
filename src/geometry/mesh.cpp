// Library includes
#include "mesh.h"

namespace noiseless
{
	void build_gpu_mesh(GPUContext context, const Mesh& mesh, GPUMesh& output)
	{
		if(!mesh.verts.empty())
		{
			output.verts = create_buffer(context, mesh.verts.size() * sizeof(Vector3), GPUBufferInfo::READ_ONLY);
			write_buffer(context, output.verts, (unsigned char*)&mesh.verts[0]);
			output.num_verts = (uint32_t)mesh.verts.size();
		}
		else
		{
			output.verts = 0;
			output.num_verts = 0;
		}

		if(!mesh.normals.empty())
		{
			output.normals = create_buffer(context, mesh.normals.size() * sizeof(Vector3), GPUBufferInfo::READ_ONLY);
			write_buffer(context, output.normals, (unsigned char*)&mesh.normals[0]);
			output.num_normals = (uint32_t)mesh.normals.size();
		}
		else
		{
			output.normals = 0;
			output.num_normals = 0;
		}

		if(!mesh.faces.empty())
		{
			output.faces = create_buffer(context, mesh.faces.size() * sizeof(IVector3), GPUBufferInfo::READ_ONLY);
			write_buffer(context, output.faces, (unsigned char*)&mesh.faces[0]);
			output.num_faces = (uint32_t)mesh.faces.size();
		}
		else
		{
			output.faces = 0;
			output.num_faces = 0;
		}
	}

	void destroy_gpu_mesh(GPUMesh& target_mesh)
	{
		if(target_mesh.num_verts)
		{
			destroy_buffer(target_mesh.verts);
		}

		if(target_mesh.num_normals)
		{
			destroy_buffer(target_mesh.normals);
		}

		if(target_mesh.num_faces)
		{
			destroy_buffer(target_mesh.faces);
		}
	}
}