#pragma once

// Internal includes
#include "resources/kernel_source.h"

namespace noiseless
{
	bool read_shader_source(const char* target_path, TKernelSource& kernelSource);
}