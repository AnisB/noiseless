// Library includes
#include "core/gpu_api.h"
#include "base/log.h"
#include "base/security.h"
#include "geometry/mesh.h"
#include "math/vector3.h"
#include "math/ivector3.h"

int main()
{
	// num faces to process
	uint32_t num_faces = 12;

	// Create a gpu context
	noiseless::GPUContext context = noiseless::create_gpu_context();

	// Create init values
	noiseless::Mesh mesh;
	mesh.verts.resize(num_faces * 3);
	for (uint32_t face_idx = 0; face_idx < num_faces; ++face_idx)
	{
		mesh.verts[3 * face_idx] = noiseless::vector3((float)face_idx, (float)(face_idx * 2), (float)(face_idx * 3));
		mesh.verts[3 * face_idx + 1] = noiseless::vector3((float)face_idx + 1, (float)((face_idx + 1) * 2), (float)((face_idx + 1) * 3));
		mesh.verts[3 * face_idx + 2] = noiseless::vector3((float)face_idx + 2, (float)((face_idx + 2) * 2), (float)((face_idx + 2) * 3));
	}

	mesh.normals.resize(num_faces * 3);
	for (uint32_t face_idx = 0; face_idx < num_faces; ++face_idx)
	{
		mesh.normals[3 * face_idx] = noiseless::vector3(0.0f, 0.0f, 1.0f);
		mesh.normals[3 * face_idx + 1] = noiseless::vector3(0.0f, 0.0f, 1.0f);
		mesh.normals[3 * face_idx + 2] = noiseless::vector3(0.0f, 0.0f, 1.0f);
	}
	
	mesh.faces.resize(num_faces);
	for (uint32_t face_idx = 0; face_idx < num_faces; ++face_idx)
	{
		mesh.faces[face_idx] = noiseless::ivector3(3 * face_idx, 3 * face_idx + 1, 3 * face_idx + 2);
	}

	noiseless::GPUMesh gpu_mesh;
	noiseless::build_gpu_mesh(context, mesh, gpu_mesh);

	// Create an output buffer
	noiseless::GPUBuffer output_buffer = noiseless::create_buffer(context, num_faces * sizeof(float), noiseless::GPUBufferInfo::WRITE_ONLY);

	// Create a program and a kernel
	noiseless::GPUProgram program = noiseless::create_program(context, "kernel/surface_area.kl");
	noiseless::GPUKernel kernel = noiseless::create_kernel(program, "process");

	// Sent the arguments
	noiseless::kernel_argument(kernel, 0, gpu_mesh.verts);
	noiseless::kernel_argument(kernel, 1, sizeof(gpu_mesh.num_verts), (unsigned char*)&gpu_mesh.num_verts);
	noiseless::kernel_argument(kernel, 2, gpu_mesh.normals);
	noiseless::kernel_argument(kernel, 3, sizeof(gpu_mesh.num_normals), (unsigned char*)&gpu_mesh.num_normals);
	noiseless::kernel_argument(kernel, 4, gpu_mesh.faces);
	noiseless::kernel_argument(kernel, 5, sizeof(gpu_mesh.num_faces), (unsigned char*)&gpu_mesh.num_faces);
	noiseless::kernel_argument(kernel, 6, output_buffer);

	// Launch the kernel
	noiseless::launch_kernel(context, kernel, num_faces);

	// Wait for the kernel to finish
	noiseless::wait_command_queue(context);

	// Read the output values
	std::vector<float> output_values(num_faces);
	noiseless::read_buffer(context, output_buffer, (unsigned char*)&output_values[0]);

	// Release all the gpu resources
	noiseless::destroy_gpu_mesh(gpu_mesh);
	noiseless::destroy_buffer(output_buffer);
	noiseless::destroy_kernel(kernel);
	noiseless::destroy_program(program);
	noiseless::destroy_gpu_context(context);
	return 0;
}