// Bento includes
#include <bento_base/log.h>
#include <bento_math/matrix4.h>
#include <bento_compute/compute_api.h>

// Internal includes
#include <model/camera.h>
#include <asset_compiler/compiler.h>
#include <resources/kernel_source.h>
#include <integrators/camera_ray_generation.h>
#include <integrators/path_tracer.h>

// External includes
#include <string>
#include <algorithm>

using namespace noiseless;

void setup_geometry(TScene& targetScene)
{
	targetScene.set_num_meshes(1);
	TGeometry& currentGeo = targetScene.geometry(0);
	currentGeo.vertices.resize(3);
	currentGeo.vertices[0] = { 10.0, 0.0, 0.0 };
	currentGeo.vertices[1] = { -10.0, 0.0, 0.0 };
	currentGeo.vertices[2] = { 0.0, 0.0, 100.0 };
	currentGeo.indices.resize(1);
	currentGeo.indices[0] = {0, 1, 2};
}

int main(int argc, char** argv)
{
	// Allocator used for this program
	bento::IAllocator& currentAlloc = *bento::common_allocator();

	// Collect the arguments
	bento::Vector<bento::DynamicString> arg_array(currentAlloc);
	arg_array.resize(argc - 1);
	for (uint32_t arg_idx = 1; arg_idx < (uint32_t)argc; ++arg_idx)
	{
		arg_array[arg_idx - 1] = argv[arg_idx];
	}

	// Build the compiler options
	TCompilerOptions options(currentAlloc);
	asset_compiler::build_from_args(options, arg_array);
	if (options._source_dir == "") return -1;

	// Compile
	bento::TAssetDatabase asset_db(*bento::common_allocator());
	bool result = asset_compiler::compile(options, asset_db);
	
	// The scene we shall be using
	TScene scene(currentAlloc);
	setup_geometry(scene);

	// Build a camera configuration
	TCamera camera;
	bento::SetIdentity(camera.view);
	camera.fov = 45;
	camera.nearPlane = 0.001f;
	camera.farPlane = 10000.0f;

	// initialize the path tracer
	TPathTracer pathTracer(currentAlloc);
	pathTracer.init_resources(asset_db);
	pathTracer.set_resolution(1920, 1080);
	pathTracer.setup_scene(scene);
	pathTracer.render(camera);

	return 0;
}