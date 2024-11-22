#pragma once

namespace IGEngine
{
	class Window;
	class RendererImpl;
}

namespace IGEngine
{
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer() = default;

		void Initialize(Window& window);
		void Render();
		void Shutdown();

	private:
		
		std::unique_ptr<RendererImpl> mImpl;
	};
}
