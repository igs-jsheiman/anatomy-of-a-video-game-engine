#include "Log.h"

#include <iostream>
#include <string>
#include <chrono>
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
		// TODO: Initialize logging system
		//OutputDebugString(L"Initialized Log System\n");
	}

	const char* Log::LogLevelToString(LogLevel Level)
	{
		// 1-to-1 mapping of log level enum to string
		return LogLevelStr[static_cast<uint32_t>(Level)];
	}

	void Log::LogMsg(const char* Msg, LogLevel Level, const char* File, int Line)
	{
        std::lock_guard<std::mutex> guard(LogMutex);

        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		std::tm localTime;
        localtime_s(&localTime, &now_time);

		// Use logLevelToString to convert the LogLevel to a string
        const char* levelStr = LogLevelToString(Level);

        std::cout << LogLevelColor[static_cast<uint32_t>(Level)] << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "] " << "[" << levelStr << "]";

//         if (file != nullptr)
//         {
//             std::cout << " [" << file << ":" << line << "]";
//         }

        std::cout << " " << Msg << "\033[0m" << std::endl;
    }
}
