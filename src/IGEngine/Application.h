#pragma once

#include "Window.h"

namespace IGEngine
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Initialize();
		void Run();

	private:

		void Update();
		void Render();

		Window m_Window;
		Log* m_Logger;
		//Renderer mRenderer;
	};

	// To be defined in a client
	Application* CreateApplication();
}
