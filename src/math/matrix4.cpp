#include "Matrix4.h"
#include "base/security.h"
// STL includes
#include <string.h>
#include <math.h>

namespace noiseless
{
	Matrix4 matrix4(float* _values)
	{
		Matrix4 result;
		memcpy(result.m, _values, sizeof(result.m));
		return result;
	}

	Matrix4 perspective(float parFovy, float parAspect, float parNear, float parFar)
	{
		Matrix4 result;
		set_identity(result);
		float f = 1.0f/tan(parFovy * 3.14f / 360.0f);
		result.m[0] = f/parAspect;
		result.m[5] = f;
		result.m[10] = (parFar+parNear)/(parNear-parFar);
		result.m[11] = (2*parFar*parNear)/(parNear-parFar);
		result.m[14] = -1;
		return result;
	}

	void set_identity(Matrix4& _mat)
	{	
		set_zero(_mat);
		_mat.m[0] = 1.0;
		_mat.m[5] = 1.0;
		_mat.m[10] = 1.0;
		_mat.m[15] = 1.0;
	}

	float determinant(const Matrix4& _mat)
	{
		//TODO
		return 0.0;
	}

	void set_zero(Matrix4& _mat)
	{
		// MEM set
		std::fill_n(_mat.m, 16, 0.0f);
	}

	Vector3 translation(const Matrix4& _mat)
	{
		return vector3(_mat.m[3],_mat.m[7],_mat.m[11]);
	}

	Vector3 x_axis(const Matrix4& _mat)
	{
		return vector3(_mat.m[0],_mat.m[4],_mat.m[8]);
	}
	Vector3 y_axis(const Matrix4& _mat)
	{
		return vector3(_mat.m[1], _mat.m[5], _mat.m[9]);
	}

	Vector3 z_axis(const Matrix4& _mat)
	{
		return vector3(_mat.m[2], _mat.m[6], _mat.m[10]);
	}

	Vector4 operator*(const Matrix4& _mat, const Vector4& _vec)
	{
		Vector4 result;
		result.x = _mat.m[0]*_vec.x + _mat.m[1]*_vec.y + _mat.m[2]*_vec.z + _mat.m[3]*_vec.w;
		result.y = _mat.m[4]*_vec.x + _mat.m[5]*_vec.y + _mat.m[6]*_vec.z + _mat.m[7]*_vec.w;
		result.z = _mat.m[8]*_vec.x + _mat.m[9]*_vec.y + _mat.m[10]*_vec.z + _mat.m[11]*_vec.w;
		result.w = _mat.m[12]*_vec.x + _mat.m[13]*_vec.y + _mat.m[14]*_vec.z + _mat.m[15]*_vec.w;
		return result;
	}

	Vector3 operator*(const Matrix4& _mat, const Vector3& _vec)
	{
		Vector3 result;
		result.x = _mat.m[0] * _vec.x + _mat.m[1] * _vec.y + _mat.m[2] * _vec.z + _mat.m[3] * 1.0f;
		result.y = _mat.m[4] * _vec.x + _mat.m[5] * _vec.y + _mat.m[6] * _vec.z + _mat.m[7] * 1.0f;
		result.z = _mat.m[8] * _vec.x + _mat.m[9] * _vec.y + _mat.m[10] * _vec.z + _mat.m[11] * 1.0f;
		return result;
	}

	Matrix4 build_translation(const Vector4& parVector)
	{
		Matrix4 result;
		result.m[0] = 1.0;
		result.m[1] = 0.0;
		result.m[2] = 0.0;
		result.m[3] = parVector.x;

		result.m[4] = 0.0;
		result.m[5] = 1.0;
		result.m[6] = 0.0;
		result.m[7] = parVector.y;
		
		result.m[8] = 0.0;
		result.m[9] = 0.0;
		result.m[10] = 1.0;
		result.m[11] = parVector.z;

		result.m[12] = 0.0;
		result.m[13] = 0.0;
		result.m[14] = 0.0;
		result.m[15] = parVector.w;
		return result;
	}


	Matrix4 build_translation(const Vector3& parVector)
	{
		Matrix4 result;
		result.m[0] = 1.0;
		result.m[1] = 0.0;
		result.m[2] = 0.0;
		result.m[3] = parVector.x;

		result.m[4] = 0.0;
		result.m[5] = 1.0;
		result.m[6] = 0.0;
		result.m[7] = parVector.y;
		
		result.m[8] = 0.0;
		result.m[9] = 0.0;
		result.m[10] = 1.0;
		result.m[11] = parVector.z;

		result.m[12] = 0.0;
		result.m[13] = 0.0;
		result.m[14] = 0.0;
		result.m[15] = 1.0;
		return result;
	}

	Matrix4 rotate_x(float parAngle)
	{
		float cosVal = cos(parAngle);
		float sinVal = sin(parAngle);
		Matrix4 result;
		result.m[0] = 1.0;
		result.m[1] = 0.0;
		result.m[2] = 0.0;
		result.m[3] = 0.0;

		result.m[4] = 0.0;
		result.m[5] = cosVal;
		result.m[6] = -sinVal;
		result.m[7] = 0.0;
		
		result.m[8] = 0.0;
		result.m[9] = sinVal;
		result.m[10] = cosVal;
		result.m[11] = 0.0;

		result.m[12] = 0.0;
		result.m[13] = 0.0;
		result.m[14] = 0.0;
		result.m[15] = 1.0;
		return result;
	}

	Matrix4 rotate_y(float parAngle)
	{
		float cosVal = cos(parAngle);
		float sinVal = sin(parAngle);
		Matrix4 result;
		result.m[0] = cosVal;
		result.m[1] = 0.0;
		result.m[2] = sinVal;
		result.m[3] = 0.0;

		result.m[4] = 0.0;
		result.m[5] = 1.0;
		result.m[6] = 0.0;
		result.m[7] = 0.0;
		
		result.m[8] = -sinVal;
		result.m[9] = 0.0;
		result.m[10] = cosVal;
		result.m[11] = 0.0;

		result.m[12] = 0.0;
		result.m[13] = 0.0;
		result.m[14] = 0.0;
		result.m[15] = 1.0;
		return result;
	}

	Matrix4 rotate_z(float parAngle)
	{
		float cosVal = cos(parAngle);
		float sinVal = sin(parAngle);
		Matrix4 result;
		result.m[0] = cosVal;
		result.m[1] = -sinVal;
		result.m[2] = 0.0;
		result.m[3] = 0.0;

		result.m[4] = sinVal;
		result.m[5] = cosVal;
		result.m[6] = 0.0;
		result.m[7] = 0.0;
		
		result.m[8] = 0.0;
		result.m[9] = 0.0;
		result.m[10] = 1.0;
		result.m[11] = 0.0;

		result.m[12] = 0.0;
		result.m[13] = 0.0;
		result.m[14] = 0.0;
		result.m[15] = 1.0;
		return result;
	}

	Matrix4 operator*(const Matrix4& _mat1, const Matrix4& _mat2 )
	{
		Matrix4 result;
		result.m[0] = _mat1.m[0] * _mat2.m[0] + _mat1.m[1] * _mat2.m[4] + _mat1.m[2] * _mat2.m[8] + _mat1.m[3] * _mat2.m[12]; 
		result.m[1] = _mat1.m[0] * _mat2.m[1] + _mat1.m[1] * _mat2.m[5] + _mat1.m[2] * _mat2.m[9] + _mat1.m[3] * _mat2.m[13]; 
		result.m[2] = _mat1.m[0] * _mat2.m[2] + _mat1.m[1] * _mat2.m[6] + _mat1.m[2] * _mat2.m[10] + _mat1.m[3] * _mat2.m[14]; 
		result.m[3] = _mat1.m[0] * _mat2.m[3] + _mat1.m[1] * _mat2.m[7] + _mat1.m[2] * _mat2.m[11] + _mat1.m[3] * _mat2.m[15]; 
		result.m[4] = _mat1.m[4] * _mat2.m[0] + _mat1.m[5] * _mat2.m[4] + _mat1.m[6] * _mat2.m[8] + _mat1.m[7] * _mat2.m[12]; 
		result.m[5] = _mat1.m[4] * _mat2.m[1] + _mat1.m[5] * _mat2.m[5] + _mat1.m[6] * _mat2.m[9] + _mat1.m[7] * _mat2.m[13]; 
		result.m[6] = _mat1.m[4] * _mat2.m[2] + _mat1.m[5] * _mat2.m[6] + _mat1.m[6] * _mat2.m[10] + _mat1.m[7] * _mat2.m[14]; 
		result.m[7] = _mat1.m[4] * _mat2.m[3] + _mat1.m[5] * _mat2.m[7] + _mat1.m[6] * _mat2.m[11] + _mat1.m[7] * _mat2.m[15]; 
		result.m[8] = _mat1.m[8] * _mat2.m[0] + _mat1.m[9] * _mat2.m[4] + _mat1.m[10] * _mat2.m[8] + _mat1.m[11] * _mat2.m[12]; 
		result.m[9] = _mat1.m[8] * _mat2.m[1] + _mat1.m[9] * _mat2.m[5] + _mat1.m[10] * _mat2.m[9] + _mat1.m[11] * _mat2.m[13]; 
		result.m[10] = _mat1.m[8] * _mat2.m[2] + _mat1.m[9] * _mat2.m[6] + _mat1.m[10] * _mat2.m[10] + _mat1.m[11] * _mat2.m[14]; 
		result.m[11] = _mat1.m[8] * _mat2.m[3] + _mat1.m[9] * _mat2.m[7] + _mat1.m[10] * _mat2.m[11] + _mat1.m[11] * _mat2.m[15]; 
		result.m[12] = _mat1.m[12] * _mat2.m[0] + _mat1.m[13] * _mat2.m[4] + _mat1.m[14] * _mat2.m[8] + _mat1.m[15] * _mat2.m[12]; 
		result.m[13] = _mat1.m[12] * _mat2.m[1] + _mat1.m[13] * _mat2.m[5] + _mat1.m[14] * _mat2.m[9] + _mat1.m[15] * _mat2.m[13]; 
		result.m[14] = _mat1.m[12] * _mat2.m[2] + _mat1.m[13] * _mat2.m[6] + _mat1.m[14] * _mat2.m[10] + _mat1.m[15] * _mat2.m[14]; 
		result.m[15] = _mat1.m[12] * _mat2.m[3] + _mat1.m[13] * _mat2.m[7] + _mat1.m[14] * _mat2.m[11] + _mat1.m[15] * _mat2.m[15]; 
		return result;
	}

	Matrix4 rotate(float _angle, const Vector3& _axis)
	{
		float u = _axis.x;
		float v = _axis.y;
		float w = _axis.z;
	    float L = (u*u + v * v + w * w);
	    float u2 = u * u;
	    float v2 = v * v;
	    float w2 = w * w; 
	 
	 	Matrix4 rotationMatrix;
	    rotationMatrix.m[0] = (u2 + (v2 + w2) * cos(_angle)) / L;
	    rotationMatrix.m[1] = (u * v * (1 - cos(_angle)) - w * sqrt(L) * sin(_angle)) / L;
	    rotationMatrix.m[2] = (u * w * (1 - cos(_angle)) + v * sqrt(L) * sin(_angle)) / L;
	    rotationMatrix.m[3] = 0.0; 
	 
	    rotationMatrix.m[4] = (u * v * (1 - cos(_angle)) + w * sqrt(L) * sin(_angle)) / L;
	    rotationMatrix.m[5] = (v2 + (u2 + w2) * cos(_angle)) / L;
	    rotationMatrix.m[6] = (v * w * (1 - cos(_angle)) - u * sqrt(L) * sin(_angle)) / L;
	    rotationMatrix.m[7] = 0.0; 
	 
	    rotationMatrix.m[8] = (u * w * (1 - cos(_angle)) - v * sqrt(L) * sin(_angle)) / L;
	    rotationMatrix.m[9] = (v * w * (1 - cos(_angle)) + u * sqrt(L) * sin(_angle)) / L;
	    rotationMatrix.m[10] = (w2 + (u2 + v2) * cos(_angle)) / L;
	    rotationMatrix.m[11] = 0.0; 
	 
	    rotationMatrix.m[12] = 0.0;
	    rotationMatrix.m[13] = 0.0;
	    rotationMatrix.m[14] = 0.0;
	    rotationMatrix.m[15] = 1.0;
	    return rotationMatrix;
	}

	std::ostream& operator<< (std::ostream& os, const Matrix4& obj) 
	{
		os << "Matrix4\n("<<obj.m[0] <<", " <<obj.m[1]<<", "<<obj.m[2]<<", " << obj.m[3]<<",\n"
			<<obj.m[4] <<", " <<obj.m[5]<<", "<<obj.m[6]<<", " << obj.m[7]<<",\n"
			<<obj.m[8] <<", " <<obj.m[9]<<", "<<obj.m[10]<<", " << obj.m[11]<<",\n"
			<<obj.m[12] <<", " <<obj.m[13]<<", "<<obj.m[14]<<", " << obj.m[15]<<")";
		return os;
	} 

	Matrix4 inverse(const Matrix4& _target )
	{
		Matrix4 result;
		const float* m = _target.m;
		result.m[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		result.m[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		result.m[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		result.m[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

		float det = m[0] * result.m[0] + m[1] * result.m[4] + m[2] * result.m[8] + m[3] * result.m[12];
		if (det == 0)
		{
			return result;
		}
		det = 1.0f / det;

		result.m[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		result.m[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		result.m[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		result.m[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] +m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		
		result.m[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		result.m[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		result.m[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		result.m[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		
		result.m[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		result.m[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		result.m[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		result.m[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

		for (int i = 0; i < 16; i++)
		{
			result.m[i] = result.m[i] * det;
		}

		return result;
	}

	const Vector4* row(const Matrix4& _matrix, uint8_t row_idx)
	{
		return reinterpret_cast<const Vector4*>(&_matrix.m[row_idx * 4]);
	}
}