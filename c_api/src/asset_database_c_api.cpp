// Internal includes
#include "asset_database_c_api.h"
#include "asset_compiler/compiler.h"

// SDK includes
#include <bento_resources/asset_database.h>

// Function to create a new noiseless asset database
NLADBObject* nl_create_adb(NLAllocatorObject* allocator)
{
	bento::IAllocator* allocPtr = (bento::IAllocator*)allocator;
	bento::TAssetDatabase* adbPtr = bento::make_new<bento::TAssetDatabase>(*allocPtr, *allocPtr);
	return (NLADBObject*)adbPtr;
;}

// Function to build an asset database from a given path
bool nl_adb_build(NLADBObject* assetDatabase, const char* assetPath, uint32_t pathLength)
{
	bento::TAssetDatabase* adbPtr = (bento::TAssetDatabase*)(assetDatabase);
	noiseless::TCompilerOptions compilerOptions(*bento::common_allocator());
	compilerOptions._source_dir = assetPath;
	return noiseless::asset_compiler::compile(compilerOptions, *adbPtr);
}

// Function to destroy a noiseless asset database
void nl_destroy_adb(NLADBObject* assetDatabase)
{
	bento::TAssetDatabase* adbPtr = (bento::TAssetDatabase*)(assetDatabase);
	bento::make_delete<bento::TAssetDatabase>(adbPtr->allocator, adbPtr);
}