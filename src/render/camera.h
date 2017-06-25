#pragma once

// Library includes
#include "math/types.h"

namespace noiseless
{
	class Camera
	{
	public:
		Camera();
		~Camera();

	public:
		void set_view(const Matrix4& target_view);
		void set_projection_data(float near, float far, float fov, float aspect_ratio);

	private:
		Matrix4 _view;
		Matrix4 _projection;
		float _near;
		float _far;
	};
}