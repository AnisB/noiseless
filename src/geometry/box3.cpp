// Library includes
#include "box3.h"
#include "math/vector3.h"

// External includes
#include <limits>
#include <algorithm>

namespace noiseless
{
	Box3::Box3()
	{
		reset();
	}

	Box3::~Box3()
	{

	}

	void Box3::reset()
	{
		min = vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		max = vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}

	void Box3::encapsulate(const Vector3* point_array, uint64_t num_points)
	{
		for(uint64_t point_index = 0; point_index < num_points; ++point_index)
		{
			const Vector3& current_point = point_array[point_index];
			
			min.x = std::min(current_point.x, min.x);
			min.y = std::min(current_point.y, min.y);
			min.z = std::min(current_point.z, min.z);

			max.x = std::max(current_point.x, max.x);
			max.y = std::max(current_point.y, max.y);
			max.z = std::max(current_point.z, max.z);
		}
	}

	void Box3::build(const Vector3* point_array, uint64_t num_points)
	{
		reset();
		encapsulate(point_array, num_points);
	}

	Vector3 Box3::diagonal()
	{
		return (max - min);
	}

	float Box3::surface_area()
	{
		if (min.x >= max.x && min.y >= max.y && min.z >= max.z)
			return 0.0f;
		const Vector3& diag = diagonal();
		return 2 * diag.x * diag.y + 2 * diag.y * diag.z + 2 * diag.z * diag.x;
	}
}