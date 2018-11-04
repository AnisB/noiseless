#pragma once

// Bento includes
#include <bento_math/types.h>

namespace noiseless
{
	struct TCamera
	{
		bento::Matrix4 view;
		float fov;
		float nearPlane;
		float farPlane;
	};
}