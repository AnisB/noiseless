#pragma once

// Internal includes
#include "bento_resources/asset_database.h"
#include "asset_compiler/compiler_options.h"

namespace noiseless {
namespace asset_compiler {

	bool compile(const TCompilerOptions& options, bento::TAssetDatabase& asset_db);
	void write(const TCompilerOptions& options, const bento::TAssetDatabase& asset_db);
}
}