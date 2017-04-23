// Library includes
#include "log.h"
#include "standard_logger.h"

namespace noiseless
{
	// This the default logger we provide for the user
	TStandardLogger __donutDefaultLogger;
	TLoggerInterface* defaultLogger = &__donutDefaultLogger;

	// Access the default logger
	TLoggerInterface* DefaultLogger()
	{
		return defaultLogger;
	}

	void SetDefaultLogger(TLoggerInterface* _loggerInterface)
	{
		defaultLogger = _loggerInterface;
	}
}