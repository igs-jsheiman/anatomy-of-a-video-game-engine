#pragma once

namespace IGEngine
{
	class Window;
	class RendererImpl;

	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer() = default;

		void Initialize(Window& window);
		void Render();
		void Shutdown();

	private:
		
		RendererImpl* m_Impl;
	};
}
