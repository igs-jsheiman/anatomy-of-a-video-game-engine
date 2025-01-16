#pragma once

namespace IGEngine
{
	class Window;
	class RendererImpl;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Initialize(Window& window);
		void Render();
		void Shutdown();

	private:
		
		//RendererImpl* m_Impl;
		std::unique_ptr<RendererImpl> m_Impl;
	};
}
