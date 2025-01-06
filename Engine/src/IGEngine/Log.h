#pragma once

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
	class Log
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

