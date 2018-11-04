// Bento includes
#include <bento_base/log.h>
#include <bento_tools/file_system.h>

// Internal includes includes
#include "resources/types.h"
#include "asset_compiler/compiler.h"
#include "asset_compiler/kernel_source_helpers.h"
#include "asset_compiler/asset_database_helpers.h"

namespace noiseless {
namespace asset_compiler {

	bool compile(const TCompilerOptions& compiler_option, bento::TAssetDatabase& asset_db)
	{
		// Flag that tracks the compilation failure or success
		bool compilation_result = true;

		bento::IAllocator& current_allocator = *bento::common_allocator();

		// Length of the source dir string (plus the slash)
		uint32_t source_dir_length = compiler_option._source_dir.size() + 1;

		// Collect and asset compile all the kernel sources
		bento::Vector<bento::DynamicString> kernels(current_allocator);
		bento::file_system::collect_files_with_extension(compiler_option._source_dir.c_str(), "kl", kernels);

		uint32_t num_shaders = kernels.size();
		for (uint32_t shader_idx = 0; shader_idx < num_shaders; ++shader_idx)
		{
			// Get the path of the shader
			const bento::DynamicString& shader_path = kernels[shader_idx];

			// Holder of the compiled asset
			TKernelSource kernelSource(current_allocator);
			bool res = read_shader_source(shader_path.c_str(), kernelSource);

			if (res)
			{
				// Insert the texture to the database
				bento::Vector<char> data(current_allocator);
				bento::pack_type(data, kernelSource);
				asset_db.insert_asset(shader_path.c_str() + source_dir_length, shader_path.c_str() + source_dir_length, TResources::ResourceType::kernel, data);
			}
			else
			{
				compilation_result = false;
				bento::default_logger()->log(bento::LogLevel::error, "ASSET_COMPILER", "Failed to compile shader");
			}
		}

		return compilation_result;
	}

	void write(const TCompilerOptions& options, const bento::TAssetDatabase& asset_db)
	{
		// Write the database and the debug file to the target_file
		bento::DynamicString output_database_path(*bento::common_allocator());
		output_database_path += options._output_dir;
		output_database_path += "/asset_database";
		noiseless::write_database(output_database_path.c_str(), asset_db);
		noiseless::write_database_debug(output_database_path.c_str(), asset_db);
	}
}
}