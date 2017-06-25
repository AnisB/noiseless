#ifndef BOX3_H
#define BOX3_H

// Library includes
#include "math/types.h"

// External includes
#include <stdint.h>

namespace noiseless
{
	class Box3
	{
	public:
		Box3();
		~Box3();

		void reset();
		void encapsulate(const Vector3* point_array, uint64_t num_points);
		void build(const Vector3* point_array, uint64_t num_points);

		Vector3 diagonal();
		float surface_area();
	public:
		Vector3 min;
		Vector3 max;
	};
}

#endif // BOX3_H