#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef IGE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace IGEngine
{
	struct WinRes
	{
		WinRes() = default;
		WinRes(uint32_t width, uint32_t height) : m_w(width), m_h(height) {}

		uint32_t m_w;
		uint32_t m_h;
	};

	class Window
	{
	public:
		Window();
		~Window();

		void Initialize(const wchar_t* windowName, const wchar_t* className, int width , int height);
		bool Update();
		void Shutdown();

		bool ProcessMessages();

		HWND GetHwnd()			const { return m_Hwnd; }
		uint32_t GetWidth()		const { return m_Rect.m_w; }
		uint32_t GetHeight()	const { return m_Rect.m_h; }

	private:
		HWND m_Hwnd;
		WinRes m_Rect;
		const wchar_t* m_ClassName;
	};

}