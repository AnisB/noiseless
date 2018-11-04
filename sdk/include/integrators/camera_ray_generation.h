#pragma once

// Internal includes
#include "model/camera.h"

namespace noiseless
{
	struct TRay
	{
		bento::Vector4 origin;
		bento::Vector4 direction;
		float tmin;
		float tmax;
	};

	struct TCameraRaySettings
	{
		bento::Vector4 origin;
		bento::Vector4 ulCorner;
		bento::Vector4 dx;
		bento::Vector4 dy;
		uint32_t width;
		uint32_t height;
		float nearPlane;
		float farPlane;
	};

	void build_ray_generation_data(const TCamera& targetCamera, uint32_t width, uint32_t height, TCameraRaySettings& outSettings);
}

