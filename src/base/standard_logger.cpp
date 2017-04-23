// Library includes
#include "standard_logger.h"

namespace noiseless
{
	TStandardLogger::TStandardLogger()
	{
	}

	TStandardLogger::~TStandardLogger()
	{
	}

	void TStandardLogger::Log(TLogLevel::Type _logLevel, const char* _message)
	{
		switch(_logLevel)
		{
			case TLogLevel::DEBUG:
			{
				std::cout<<"[DEBUG]";
			}
			break;
			case TLogLevel::INFO:
			{
				std::cout<<"[INFO]";
			}
			break;
			case TLogLevel::WARNING:
			{
				std::cout<<"[WARNING]";
			}
			break;
			case TLogLevel::ERROR:
			{
				std::cout<<"[ERROR]";
			}
			break;
		};

		std::cout<<_message<<std::endl;
	}
}