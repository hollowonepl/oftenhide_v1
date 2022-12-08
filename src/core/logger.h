
#ifndef USHINE_CORE_LOGGER
#define USHINE_CORE_LOGGER

#include "common.h"

namespace ushine
{
	namespace core 
	{

	class DYNLIB_EXPORT LoggerOutput
	{
	public:
		LoggerOutput()
		{

		}
		//LoggerOutput(LoggerOutput&& copyOut)
		//{
		//	configuration = copyOut.configuration;
		//}

		virtual int Open(std::string config) { return 1; }
		virtual int Close() { return 1; }
		virtual int StreamOut(std::string message) { return 1; }
	private:
		std::string configuration;
	};
	//class ::LoggerOutput


	class DYNLIB_EXPORT Logger {

	public:
		Logger();
		Logger(LoggerOutput& output);
		virtual ~Logger();

		int ConfigureOutPut(LoggerOutput& configuration);
		int LogLine(std::string logLine);

	private:
		class pimpl;
		std::unique_ptr<pimpl> impl;

	}; //class ::Logger
	}; //namespace 	 ::core::
}; //namespace ushine::

#endif