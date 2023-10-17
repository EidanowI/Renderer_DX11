#pragma once
#include <Windows.h>
#include <wrl.h>//for Microsoft::WRL
///TODO Window Create Propertices with ID of Graphics card

#include <string>
#include <optional>

#include "DX11_ErrorMacro.h"

class Window {
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
public:
	Window(unsigned int width, unsigned int height, const wchar_t* windowName) noexcept;
	~Window() noexcept;
	HWND GetHinstnce() const noexcept {
		return S_hWnd;
	}

	void SetWindowTitle(const std::wstring& title) noexcept;
	static std::optional<int> ProcessMsg() noexcept;

private:
	static unsigned short S_WindowWidth;
	static unsigned short m_WindowHeight;
	static HWND S_hWnd;
};
