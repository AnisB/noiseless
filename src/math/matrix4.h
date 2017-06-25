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
#ifndef MATH_MATRIX4_H
#define MATH_MATRIX4_H

// Library includes
#include "vector3.h"
#include "vector4.h"

// STL includes
#include <iostream>

namespace noiseless
{
	// Init Functions
	Matrix4 matrix4(float* _values);

	void set_identity(Matrix4& _matrix);
	void set_zero(Matrix4& _matrix);
	void set_perspective(Matrix4& matrix, float parFovy, float parAspect, float parNear, float parFar);

	Vector3 translation(const Matrix4& _matrix);
	Vector3 x_axis(const Matrix4& _matrix);
	Vector3 y_axis(const Matrix4& _matrix);
	Vector3 z_axis(const Matrix4& _matrix);
	const Vector4* row(const Matrix4& _matrix, uint8_t);
	
	float determinant(const Matrix4& _matrix);
	Matrix4 inverse(const Matrix4& _target);
	Matrix4 build_translation(const Vector4& parVector);
	Matrix4 build_translation(const Vector3& parVector);

	Matrix4 rotate(float parAngle, const Vector3& parAxis);

	Matrix4 rotate_x(float parAngle);
	Matrix4 rotate_y(float parAngle);
	Matrix4 rotate_z(float parAngle);

	Vector4 operator*(const Matrix4& _mat, const Vector4& _fac);
	Vector3 operator*(const Matrix4& _mat, const Vector3& _fac);
	Matrix4 operator*(const Matrix4& _mat1, const Matrix4& _mat2);
	std::ostream& operator<< (std::ostream& os, const Matrix4& obj); 
}
#endif // MATH_MATRIX4_H
