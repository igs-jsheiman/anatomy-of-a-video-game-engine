#pragma once

namespace IGEngine
{
	class Window;
	class RendererImpl;

	class Renderer : public Singleton<Renderer>
	{
	public:
		Renderer();
		~Renderer();

		void Initialize(Window& window);
		void Render();
		void Shutdown();

	private:
		
		std::unique_ptr<RendererImpl> m_Impl;
	};
}
