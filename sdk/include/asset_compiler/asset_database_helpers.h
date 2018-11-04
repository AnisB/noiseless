#pragma once

// Internal includes
#include "bento_resources/asset_database.h"

namespace noiseless
{
	// Helpers to write the binary version of the database
	bool write_database(const char* target_path, const bento::TAssetDatabase& target_database);
	bool read_database(const char* target_path, bento::TAssetDatabase& output_database);

	// Helpers to write the debug info of the database
	bool write_database_debug(const char* target_path, const bento::TAssetDatabase& target_database);
}