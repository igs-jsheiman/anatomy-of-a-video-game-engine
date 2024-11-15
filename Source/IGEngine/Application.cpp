#include "igpch.h"
#include "Application.h"
#include "Events.h"
#include "Log.h"

#define DEBUG_LOGGER 0

namespace IGEngine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		// Initialize the log functionality
		IGEngine::Log* logger = IGEngine::Log::GetInstance();
		logger->Init();

#if DEBUG_LOGGER
		LOG_INFO(logger, "This is an informational message.");
		LOG_DEBUG(logger, "This is a debug message.");
		LOG_ERROR(logger, "This is an error message.");
		LOG_WARNING(logger, "This is a warning message.");
		LOG_CRITICAL(logger, "This is a critical message.");

		LOG_FILE_INFO(logger, "This is an informational message.");
		LOG_FILE_DEBUG(logger, "This is a debug message.");
		LOG_FILE_ERROR(logger, "This is an error message.");
		LOG_FILE_WARNING(logger, "This is a warning message.");
		LOG_FILE_CRITICAL(logger, "This is a critical message.");
#endif

		WindowResizeEvent e(1280, 720);
		StrSmartPtr windowEventInfo = e.GetDetailedInfo();
		LOG_INFO(logger, windowEventInfo.get());

		while (true);
	}
}
