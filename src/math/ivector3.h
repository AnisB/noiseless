#ifndef BUTTER_IVECTOR3_H
#define BUTTER_IVECTOR3_H

#include "types.h"

namespace noiseless
{
	// inline methods
		// Constructors
	inline IVector3 ivector3(const int _x,const int _y, const int _z);
	inline IVector3 ivector3(const int _v);

		// Index access
	inline int& at_index(IVector3& _val, int _index);
	inline const int& at_index(const IVector3& _val, int _index);
}

#include "ivector3.inl"

#endif // BUTTER_IVECTOR3_H
