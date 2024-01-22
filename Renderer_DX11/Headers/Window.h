#pragma once

#include <Windows.h>

#include <wrl.h>//for Microsoft::WRL
#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_impl_win32.h"
#include "../Dependencies/imGUI/imgui_impl_dx11.h"

#include "DX11_ErrorMacro.h"
#include "InputSystem.h"

#include <string>
#include <optional>

/// <summary>
/// Register Window class and create window. Also processes window messages.
/// </summary>
class Window {
	friend class Graphics;
	friend LRESULT WindowProcess(HWND, UINT, WPARAM, LPARAM);

	struct CreateWindowParams {
		int deviceID = -1;
	};

	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept {
			return S_wndClassName;
		}
		static HINSTANCE GetInstance() noexcept {
			return S_wndClass.m_hInst;
		}

	private:
		WindowClass() noexcept;
		~WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static WindowClass S_wndClass;
		static const wchar_t* S_wndClassName;
		HINSTANCE m_hInst;
	};

private:
	void InitializeRawDevices() noexcept;

public:
	Window(unsigned int width, unsigned int height, const wchar_t* windowName) noexcept;
	~Window() noexcept;
	HWND GetHinstnce() const noexcept {
		return S_hWnd;
	}

	static unsigned short GetWindowWidth() noexcept {
		return S_WindowWidth;
	}
	static unsigned short GetWindowHeight() noexcept {
		return S_WindowHeight;
	}

	static void SetWindowTitle(const std::wstring& title) noexcept;
	static void EnableCursor() noexcept;
	static void DisableCursor() noexcept;
	static void FixateCursor() noexcept;
	static void FreeCursor() noexcept;

	static std::optional<int> ProcessMsg() noexcept;

private:
	static unsigned short S_WindowWidth;
	static unsigned short S_WindowHeight;
	static HWND S_hWnd;

public:
	static CreateWindowParams S_CreateWindowParams;
};