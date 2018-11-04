// Internal includes
#include "integrators/camera_ray_generation.h"

// bento includes
#include <bento_math/const.h>
#include <bento_math/matrix4.h>

namespace noiseless
{
	void build_ray_generation_data(const TCamera& targetCamera, uint32_t width, uint32_t height, TCameraRaySettings& outSettings)
	{
		const bento::Vector3& position = GetTranslate(targetCamera.view);
		const bento::Vector3& xaxis = XAxis(targetCamera.view);
		const bento::Vector3& yaxis = YAxis(targetCamera.view);
		const bento::Vector3& zaxis = ZAxis(targetCamera.view);

		// Compute the screen height
		float halfScreenHeight = tan(targetCamera.fov * PI_180 * 0.5f);

		// Compute the screen ratio of the screen
		float aspectRatio = width/(float)height;

		// Compute the screen center position
		const bento::Vector3& screenCenter = position + zaxis;

		// Fill the settings
		outSettings.origin = bento::vector4(position);
		outSettings.ulCorner = bento::vector4(screenCenter + yaxis * halfScreenHeight + xaxis * halfScreenHeight * aspectRatio);
		outSettings.dx = bento::vector4(-xaxis * halfScreenHeight * 2.0 * aspectRatio / (float)width);
		outSettings.dy = bento::vector4(-yaxis * halfScreenHeight * 2.0 / (float)height);
		outSettings.width = width;
		outSettings.height = height;
		outSettings.nearPlane = targetCamera.nearPlane;
		outSettings.farPlane = targetCamera.farPlane;
	}
}