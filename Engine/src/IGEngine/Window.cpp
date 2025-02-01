#include "igspch.h"
#include "Window.h"

#include <assert.h>

namespace IGEngine
{

	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch (umessage)
		{
			case WM_KEYDOWN:
			{
				if (wparam == VK_ESCAPE)
				{
					PostQuitMessage(0);
					return 0;
				}
				else
				{
					return DefWindowProc(hwnd, umessage, wparam, lparam);
				}
			}
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				return 0;
			case WM_PAINT:
// 				if (pSample)
// 				{
// 					pSample->OnUpdate();
// 					pSample->OnRender();
// 				}
				return 1;
			case WM_DESTROY:
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}
			default:
				return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}


	void Window::Initialize(const wchar_t* windowName, const wchar_t* className, int width, int height)
	{
		// Create and open the main window
		uint32_t windowWidth = width;
		uint32_t windowHeight = height;
		HINSTANCE moduleHandle = GetModuleHandle(nullptr);

		WNDCLASSEXW wc = { 0 };
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = moduleHandle;
		wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className;
		wc.cbSize = sizeof(WNDCLASSEXW);
		RegisterClassExW(&wc);

		m_Hwnd = CreateWindowExW(WS_EX_APPWINDOW,
			className,
			windowName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_SIZEBOX,
			(GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2, windowWidth, windowHeight,
			nullptr, 
			nullptr, 
			moduleHandle, 
			nullptr);

		m_Rect = WinRes(width, height);

		ShowWindow(m_Hwnd, SW_SHOW);
		SetForegroundWindow(m_Hwnd);
		SetFocus(m_Hwnd);
		ShowCursor(true);

		m_ClassName = className;
	}

	bool Window::Update()
	{
		return ProcessMessages();
	}

	void Window::Shutdown()
	{
		HINSTANCE moduleHandle = GetModuleHandle(nullptr);

		DestroyWindow(m_Hwnd);
		m_Hwnd = nullptr;

		UnregisterClass(m_ClassName, moduleHandle);
		moduleHandle = nullptr;
	}

	bool Window::ProcessMessages()
	{
		bool running = true;
		MSG msg = {};
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return running;
	}
}