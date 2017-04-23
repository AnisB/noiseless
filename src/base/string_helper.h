#ifndef BASE_STRING_HELPER_H
#define BASE_STRING_HELPER_H

// External includes
#include <string.h>
#include <sstream>
#include <vector>

namespace noiseless
{
	// The string base and types (in case i'd like to move to wstring or whatever)
	#define STRING_TYPE std::string
	#define CHAR_TYPE char
	#define STREAM_TYPE std::stringstream

	// Generic converter to string
	template <typename T>
	STRING_TYPE convert_to_string(const T& value)
	{
		STREAM_TYPE stream;
		stream << value;
		return stream.str();
	}

	// Generic converter from string
	template <typename T>
	T convert_from_string(const STRING_TYPE& str)
	{
		STREAM_TYPE stream(str);
		T val;
		stream >> val;
		return val;
	}

	// Takes a string and removes multiple spaces from it
	void remove_mult_spaces(STRING_TYPE& str);
}

#endif // BASE_STRING_HELPER_H