#pragma once

#include "types_c_api.h"

extern "C"
{
	// Function to create a new noiseless asset database
	NL_EXPORT NLADBObject* nl_create_adb(NLAllocatorObject* allocator);

	// Function to build an asset database from a given path
	NL_EXPORT bool nl_adb_build(NLADBObject* assetDatabase, const char* path, uint32_t pathLength);

	// Function to destroy a noiseless asset database
	NL_EXPORT void nl_destroy_adb(NLADBObject* assetDatabsae);
}