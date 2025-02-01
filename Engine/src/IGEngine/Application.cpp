#include "igspch.h"
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
		g_Logger->Init();
	}

	void Application::Run()
	{

#if DEBUG_LOGGER
		LOG_INFO(m_Logger, "This is an informational message.");
		LOG_DEBUG(m_Logger, "This is a debug message.");
		LOG_ERROR(m_Logger, "This is an error message.");
		LOG_WARNING(m_Logger, "This is a warning message.");
		LOG_CRITICAL(m_Logger, "This is a critical message.");

		LOG_FILE_INFO(m_Logger, "This is an informational message.");
		LOG_FILE_DEBUG(m_Logger, "This is a debug message.");
		LOG_FILE_ERROR(m_Logger, "This is an error message.");
		LOG_FILE_WARNING(m_Logger, "This is a warning message.");
		LOG_FILE_CRITICAL(m_Logger, "This is a critical message.");
#endif

		// Window Resize Event...
		WindowResizeEvent resizeEvent(1280, 720);
		//SmartPointer<char[]> windowEventInfo = resizeEvent.GetDetailedInfo();
		//LOG_INFO(g_Logger, windowEventInfo.get());

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
		// Always clear out stack allocators at the start of the frame
		g_MemoryManager.ClearStackAllocator();

		char* testInt = g_MemoryManager.SingleFrameAllocate<char>(sizeof(char) * 128, 4);
		snprintf(testInt, sizeof(char) * 128, "This is a test of the single frame allocator. Frame: %llu", m_Frame);
		LOG_INFO(g_Logger, testInt);

		m_Frame++;
	}

	void Application::Render()
	{
		m_Renderer.Render();
	}
}
