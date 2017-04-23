// Library includes
#include "common.h"
#include "system_allocator.h"

namespace noiseless
{
	// This variable is used to process all common memory space allocation (if no particular behavior)
	// is requested, this is only new that is tolered in the whole library.
	static TSystemAllocator commonAllocator;

	// Return the common allocator ptr
	TAllocator* basic_allocator()
	{
		return &commonAllocator;
	}
}