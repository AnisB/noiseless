// Library includes
#include "string_helper.h"
#include <algorithm>

// External includes
#include <vector>

namespace noiseless
{
	bool both_are_spaces(CHAR_TYPE lhs, CHAR_TYPE rhs) 
	{ 
		return (lhs == rhs) && (lhs == ' '); 
	}

	void remove_mult_spaces(STRING_TYPE& str) 
	{
		STRING_TYPE::iterator new_end = std::unique(str.begin(), str.end(), both_are_spaces);
		str.erase(new_end, str.end());  
	}
}