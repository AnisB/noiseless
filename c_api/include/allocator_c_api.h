#pragma once

#include "types_c_api.h"

extern "C"
{
	// Function to create a new noiseless allocator
	NL_EXPORT NLAllocatorObject* nl_create_allocator();

	// Function to destroy a noiseless allocator
	NL_EXPORT void nl_destroy_allocator(NLAllocatorObject* allocator);
}