// Internal includes
#include "asset_compiler/kernel_source_helpers.h"

// Bento include
#include <bento_tools/file_system.h>

namespace noiseless
{
	bool read_shader_source(const char* target_path, TKernelSource& kernelSource)
	{
		return bento::read_file(target_path, kernelSource.data, bento::FileType::Text);
	}
}