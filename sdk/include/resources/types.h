#pragma once

// External includes
#include <stdint.h>

namespace noiseless
{
	struct TResources
	{
		enum ResourceType
		{
			kernel = 0,
			none = 1
		};
		static const char* resource_type_to_string(ResourceType type);
	};

}