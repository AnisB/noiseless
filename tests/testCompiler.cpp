// Bento includes
#include <asset_compiler/compiler.h>

int main(int argc, char** argv)
{
	// Allocator used for this program
	bento::IAllocator& current_alloc = *bento::common_allocator();

	// Collect the arguments
	bento::Vector<bento::DynamicString> arg_array(current_alloc);
	arg_array.resize(argc);
	for (uint32_t arg_idx = 0; arg_idx < (uint32_t)argc; ++arg_idx)
	{
		arg_array[arg_idx] = argv[arg_idx];
	}

	// Build the compiler options
	noiseless::TCompilerOptions options(current_alloc);
	noiseless::asset_compiler::build_from_args(options, arg_array);
	if (options._source_dir == "") return -1;

	// Compile
	bento::TAssetDatabase asset_db(*bento::common_allocator());
	bool result = noiseless::asset_compiler::compile(options, asset_db);

	// Write it to the disk
	if (options._output_dir == "") return -2;
	noiseless::asset_compiler::write(options, asset_db);

	return 0;
}