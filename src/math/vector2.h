/**
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**/
#ifndef BUTTER_VECTOR2_H
#define BUTTER_VECTOR2_H

#include "types.h"

namespace noiseless
{
	// inline methods
		// Constructors
	inline Vector2 vector2(const float _x,const float _y);
	inline Vector2 vector2(const float _v);

		// Operators
	inline Vector2 operator*(const Vector2& _v1, float parFactor);
	inline Vector2 operator/(const Vector2& _v1, float parFactor);
	inline Vector2 operator+(const Vector2& _v1, const Vector2& _v2);
	inline Vector2 operator-(const Vector2& _v1, const Vector2& _v2);

		// Cannonic operations
	inline float dotProd(const Vector2& _v1, const Vector2& _v2);
	inline float length(const Vector2& _v1);
	inline float length2(const Vector2& _v1);

	// Default values;
	extern const Vector2 v2_ZERO;
	extern const Vector2 v2_X;
	extern const Vector2 v2_Y;
	extern const Vector2 v2_Z;
}

#include "vector2.inl"

#endif // BUTTER_VECTOR2_H