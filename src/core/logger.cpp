#include <iostream>
#include <string>
#include "logger.h"


namespace ushine
{
	namespace core 
	{


		class Logger::pimpl
		{
		public:
			std::string getLogDate()
			{
				return std::string("2015-01-01 00:00:00");
			}

			LoggerOutput Out;
			int LogEntry = 1;
		};

		Logger::Logger() : impl(new pimpl)
		{		
			//impl.Out = output;
		}

		Logger::Logger(LoggerOutput& output) : impl(new pimpl)
		{		
			//impl.Out = output;
		}	

		Logger::~Logger()
		{
		}

		int Logger::LogLine(std::string logLine)
		{
			std::cout << impl->LogEntry <<  ")\t [" << impl->getLogDate() << "]\t" << logLine << std::endl;
			impl->LogEntry++;
			return 0;
		}


	} //namespace 	 ::core
}; //namespace ushine::


