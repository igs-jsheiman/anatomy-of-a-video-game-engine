#pragma once

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
 		Renderer m_Renderer;

		uint64_t m_Frame = 0;
	};

	// To be defined in a client
	Application* CreateApplication();
}
