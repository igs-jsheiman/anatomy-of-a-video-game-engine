#pragma once

#include "Core.h"

#include <mutex>

// Must instantiate stl classes for dll prior to usage
class IGENGINE_API std::mutex;

namespace IGEngine
{
	// Define the LogLevel enum to specify the severity of the log messages.
	enum LogLevel
	{
		DEBUG = 0,
		INFO,
		WARNING,
		ERROR,
		CRITICAL
	};

	const char* LogLevelStr[] =
	{
		"DEBUG",
		"INFO",
		"WARNING",
		"ERROR",
		"CRITICAL"
	};

	const char* LogLevelColor[] =
	{
		"\033[36m", // Cyan
		"\033[0m",	// White
		"\033[33m", // Yellow
		"\033[31m", // Red
		"\033[35m", // Magenta
	};

#define LOG(logger, level, message) (logger)->LogMsg(message, level, __FILE__, __LINE__, false)
#define LOG_FILE(logger, level, message) (logger)->LogMsg(message, level, __FILE__, __LINE__, true)

#define LOG_DEBUG(logger, message) LOG(logger, IGEngine::LogLevel::DEBUG, message)
#define LOG_INFO(logger, message) LOG(logger, IGEngine::LogLevel::INFO, message)
#define LOG_WARNING(logger, message) LOG(logger, IGEngine::LogLevel::WARNING, message)
#define LOG_ERROR(logger, message) LOG(logger, IGEngine::LogLevel::ERROR, message)
#define LOG_CRITICAL(logger, message) LOG(logger, IGEngine::LogLevel::CRITICAL, message)

#define LOG_FILE_DEBUG(logger, message) LOG_FILE(logger, IGEngine::LogLevel::DEBUG, message)
#define LOG_FILE_INFO(logger, message) LOG_FILE(logger, IGEngine::LogLevel::INFO, message)
#define LOG_FILE_WARNING(logger, message) LOG_FILE(logger, IGEngine::LogLevel::WARNING, message)
#define LOG_FILE_ERROR(logger, message) LOG_FILE(logger, IGEngine::LogLevel::ERROR, message)
#define LOG_FILE_CRITICAL(logger, message) LOG_FILE(logger, IGEngine::LogLevel::CRITICAL, message)

	// Single instance of a log class for all logging purposes.
	// There is no reason to have multiple copies of this class which is why it is a singleton.
	class IGENGINE_API Log
	{
	public:
		static Log* GetInstance()
		{
			if (LogInstance == nullptr)
			{
				LogInstance = new Log();
			}
			return LogInstance;
		}

		void Init();
		void LogMsg(const char* Msg, LogLevel Lvl, const char* File, int Line, bool bWriteToFile);

		~Log()
		{
			delete this;
		}

	protected:
		std::mutex LogMutex;
		const char* LogLevelToString(LogLevel Lvl);

	private:
		static Log* LogInstance;

		void LogConsole(const char* Msg, LogLevel Lvl, const std::tm& LocalTime, const char* File, int Line);
		void LogFile(const char* Msg, LogLevel Lvl, const std::tm& LocalTime, const char* File, int Line);

		// Private constructor disallows anyone creating multiple instances of the Log class
		Log() {}
		// Remove ability to use copy constructor and assignment operator as well
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
	};
}

