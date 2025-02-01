#pragma once

#include "Singleton.h"

// Must instantiate stl classes for dll prior to usage
// class IGENGINE_API std::mutex;

// Log class based on source code from article: https://araujo88.medium.com/devlog-writing-a-logging-library-in-c-9601ec3699eb

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

#define LOG(logger, level, message) (logger)->LogMsg(message, level, __FILE__, __LINE__, false)
#define LOG_FILE(logger, level, message) (logger)->LogMsg(message, level, __FILE__, __LINE__, true)

#define LOG_DEBUG(logger, message) LOG(logger, LogLevel::DEBUG, message)
#define LOG_INFO(logger, message) LOG(logger, LogLevel::INFO, message)
#define LOG_WARNING(logger, message) LOG(logger, LogLevel::WARNING, message)
#define LOG_ERROR(logger, message) LOG(logger, LogLevel::ERROR, message)
#define LOG_CRITICAL(logger, message) LOG(logger, LogLevel::CRITICAL, message)

#define LOG_FILE_DEBUG(logger, message) LOG_FILE(logger, LogLevel::DEBUG, message)
#define LOG_FILE_INFO(logger, message) LOG_FILE(logger, LogLevel::INFO, message)
#define LOG_FILE_WARNING(logger, message) LOG_FILE(logger, LogLevel::WARNING, message)
#define LOG_FILE_ERROR(logger, message) LOG_FILE(logger, LogLevel::ERROR, message)
#define LOG_FILE_CRITICAL(logger, message) LOG_FILE(logger, LogLevel::CRITICAL, message)

	// Single instance of a log class for all logging purposes.
	// There is no reason to have multiple copies of this class which is why it is a singleton.
	class Log : public Singleton<Log>
	{
	public:

		void Init();
		void LogMsg(const char* Msg, LogLevel Lvl, const char* File, int Line, bool bWriteToFile);

	protected:
		std::mutex LogMutex;
		const char* LogLevelToString(LogLevel Lvl);

	private:
		static Log* LogInstance;

		void LogConsole(const char* Msg, LogLevel Lvl, const std::tm& LocalTime, const char* File, int Line);
		void LogFile(const char* Msg, LogLevel Lvl, const std::tm& LocalTime, const char* File, int Line);
	};

	static Log* g_Logger = Log::GetInstance();
}

