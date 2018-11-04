// Internal includes
#include "resources/types.h"

namespace noiseless
{
	const char* TResources::resource_type_to_string(ResourceType type)
	{
		switch (type)
		{
			case ResourceType::kernel:
			{
				return "kernel";
			}
			break;
			default:
				return "none";
		};
	}
}