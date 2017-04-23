#ifndef STANDARD_LOGGER_H
#define STANDARD_LOGGER_H

// Library includes
#include "log.h"

namespace noiseless
{
	class TStandardLogger : public TLoggerInterface
	{
	public:
		TStandardLogger();
		~TStandardLogger();

		// The virtual logging function that must be 
		virtual void Log(TLogLevel::Type _logLevel, const char* _message);
	};
}

#endif // STANDARD_LOGGER_H