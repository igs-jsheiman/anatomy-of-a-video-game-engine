#include "Log.h"

#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>

namespace IGEngine
{
	// Initialize the static member variable to a null pointer
	// so the first time anything asks for the instance it will create it.
	Log* Log::LogInstance = nullptr;

	void Log::Init()
	{
		LogMsg("Initialized!", INFO, nullptr, 0, false);
	}

	const char* Log::LogLevelToString(LogLevel Level)
	{
		// 1-to-1 mapping of log level enum to string
		return LogLevelStr[static_cast<uint32_t>(Level)];
	}

	void Log::LogMsg(const char* Msg, LogLevel Lvl, const char* File, int Line, bool bWriteToFile)
	{
		// Protect multiple log messages from occurring at the same time using mutex
        std::lock_guard<std::mutex> guard(LogMutex);

		// Get the current local time
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		std::tm localTime;
        localtime_s(&localTime, &now_time);

		if (bWriteToFile)
		{
			LogFile(Msg, Lvl, localTime, File, Line);
		}
		else
		{
			LogConsole(Msg, Lvl, localTime, File, Line);
		}
    }

	void Log::LogConsole(const char* Msg, LogLevel Lvl, const std::tm& LocalTime, const char* File, int Line)
	{
		const char* levelStr = LogLevelToString(Lvl);

		if (File)
		{
			std::cout << " [" << File << ":" << Line << "]";
		}
		std::cout << LogLevelColor[static_cast<uint32_t>(Lvl)] << "[" << std::put_time(&LocalTime, "%Y-%m-%d %H:%M:%S") << "] " << "[" << levelStr << "]";
		std::cout << " " << Msg << "\033[0m" << std::endl;
	}

	void Log::LogFile(const char* Msg, LogLevel Lvl, const std::tm& LocalTime, const char* File, int Line)
	{
		const char* levelStr = LogLevelToString(Lvl);

		// Write out to disk instead of std::cout
		std::ofstream logFile;
		logFile.open("log.txt", std::ios::out | std::ios::app);
		if (!logFile.is_open())
		{
			std::cerr << "Failed to open log file: log.txt" << std::endl;
		}

		// Format and write the log message
		logFile << "[" << std::put_time(&LocalTime, "%Y-%m-%d %H:%M:%S") << "] " << "[" << levelStr << "] ";

		if (File != nullptr)
		{
			logFile << "[" << File << ":" << Line << "] ";
		}

		logFile << Msg << std::endl;

		logFile.close();
	}
}
