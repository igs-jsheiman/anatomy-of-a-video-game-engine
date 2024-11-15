#pragma once

#ifdef IGE_PLATFORM_WINDOWS

extern IGEngine::Application* IGEngine::CreateApplication();

int main(int argc, char** argv)
{
	IGEngine::Log* logger = IGEngine::Log::GetInstance();
	logger->Init();

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

	auto app = IGEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif