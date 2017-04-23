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

#ifndef LOG_SYSTEM_H
#define LOG_SYSTEM_H

// Library includes
#include "string_helper.h"

// STL Includes
#include <iostream>
#include <cstdio>

namespace noiseless
{
	// Defining the different available log levels
	namespace TLogLevel
	{
		enum Type
		{
			DEBUG = 0,
			INFO = 1, 
			WARNING = 2,
			ERROR = 3,
		};	
	}

	// The logger interface that must be overloaded by your custom logger
	class TLoggerInterface
	{
	public:
		TLoggerInterface() {}
		virtual ~TLoggerInterface() {}

		// The virtual logging function that must be 
		virtual void Log(TLogLevel::Type _logLevel, const char* _message) = 0;
	};

	// Access the default logger
	TLoggerInterface* DefaultLogger();
	void SetDefaultLogger(TLoggerInterface* _loggerInterface);

	// General print macro
	#define PRINT_GENERAL(LEVEL, ENONCE)\
	{\
		STREAM_TYPE collector;\
		collector << ENONCE;\
		noiseless::TLoggerInterface* logger = noiseless::DefaultLogger();\
		logger->Log(LEVEL, collector.str().c_str());\
	}

	#if _DEBUG
		#define PRINT_DEBUG(ENONCE) PRINT_GENERAL(noiseless::TLogLevel::DEBUG, ENONCE)
	#else
		#define PRINT_DEBUG(ENONCE) {}
	#endif
	
	#define PRINT_INFO(ENONCE) PRINT_GENERAL(noiseless::TLogLevel::INFO, ENONCE)
	#define PRINT_WARNING(ENONCE) PRINT_GENERAL(noiseless::TLogLevel::WARNING, ENONCE)
	#define PRINT_ERROR(ENONCE) PRINT_GENERAL(noiseless::TLogLevel::ERROR, ENONCE)
}

#endif // LOG_SYSTEM_H