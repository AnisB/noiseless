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
#ifndef MATH_TYPES_H
#define MATH_TYPES_H

namespace noiseless
{
	// Vector types
	// floats
	struct Vector2
	{
		float x,y;
	};
	struct Vector3
	{
		float x,y,z;
	};
	struct Vector4
	{
		float x,y,z,w;
	};

	// Int
	struct IVector2
	{
		int x,y;
	};
	struct IVector3
	{
		int x,y,z;
	};
	struct IVector4
	{
		int x,y,z,w;
	};

	struct Matrix3
	{
		float m[9];
	};
	
	struct Matrix4
	{
		float m[16];
	};
}

#endif // MATH_TYPES_H