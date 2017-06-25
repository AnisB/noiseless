// Linbrary includes
#include "camera.h"
#include "math/matrix4.h"

namespace noiseless
{
	Camera::Camera()
	{

	}

	Camera::~Camera()
	{

	}


	void Camera::set_view(const Matrix4& target_view)
	{
		_view = target_view;
	}

	void Camera::set_projection_data(float near, float far, float fov, float aspect_ratio)
	{
		_near = near;
		_far = far;

		// Compute the perspective matrix
		set_perspective(_projection, fov, aspect_ratio, near, far);
	}
}