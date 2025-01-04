#pragma once

namespace IGEngine
{
<<<<<<< Updated upstream

	class IGENGINE_API Application
=======
	class Application
>>>>>>> Stashed changes
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
		Renderer m_Renderer;
	};

	// To be defined in a client
	Application* CreateApplication();
}
