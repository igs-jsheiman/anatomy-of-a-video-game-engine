#include "igPCH.h"
#include "Application.h"


#define DEBUG_LOGGER 0

namespace IGEngine
{
	Application::Application() : m_Window()
	{
		Initialize();
	}

	Application::~Application()
	{
	}

	void Application::Initialize()
	{
		// Initialize the main window...
		m_Window.Initialize(L"Bird Game", L"Bird Game", 1280, 720);

		m_Renderer.Initialize(m_Window);

		// Initialize the logging...
		m_Logger = IGEngine::Log::GetInstance();
		m_Logger->Init();
	}

	void Application::Run()
	{

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

		// Window Resize Event...
		WindowResizeEvent resizeEvent(1280, 720);
		//SmartPointer<char[]> windowEventInfo = resizeEvent.GetDetailedInfo();
		//LOG_INFO(m_Logger, windowEventInfo.get());

		{
			SmartChar test;
		}

		while (m_Window.Update())
		{
			Update();
			Render();
		}
	
		m_Window.Shutdown();
	}

	void Application::Update()
	{
	}

	void Application::Render()
	{
		m_Renderer.Render();
	}
}
