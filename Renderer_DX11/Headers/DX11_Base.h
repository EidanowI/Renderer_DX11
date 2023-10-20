#pragma once
#include <Windows.h>
#include <wrl.h>//for Microsoft::WRL
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_win32.h"
#include "../imGUI/imgui_impl_dx11.h"
///TODO Window Create Propertices with ID of Graphics card

#include <d3d11.h>

#include <string>
#include <optional>
#include <vector>

#include "DX11_ErrorMacro.h"


extern bool SHOULD_CLOSE_WINDOW_AND_CREATE_NEW;
class Window {
	friend class Graphics;
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
	static unsigned short S_WindowHeight;
	static HWND S_hWnd;
public:
	static CreateWindowParams S_CreateWindowParams;
};
class ImGUIManager {
public:
	ImGUIManager() noexcept;
	~ImGUIManager() noexcept;

	static void NewFrame() noexcept;
	static void Render() noexcept;
	static void ShowGUIWindows() noexcept;
private:
	static bool S_isCreated;
	static bool S_isShowGraphicsSetupWindow;
};
class Graphics {
	struct GraphicAdapterInfo {
		unsigned int deviceID;
		std::wstring adapterName;
		int score;
	};
	static void GetDevices(const std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters, Graphics::GraphicAdapterInfo* adapterInfos) noexcept;
	static IDXGIAdapter* GetMostPowerfulAdapter(const std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters, Graphics::GraphicAdapterInfo* adapterInfos) noexcept;
	static IDXGIAdapter* GetAdapterByID(const std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters) noexcept;
public:
	Graphics() noexcept = default;

	static void Initialize(HWND hWnd, const void* pWindow) noexcept;
	static void UnInitialize() noexcept;

	static ID3D11Device* GetDevice() noexcept {
		return S_pDevice;
	}
	static ID3D11DeviceContext* GetDeviceContext() noexcept {
		return S_pDeviceContext;
	}

	static void SetRenderTargets() noexcept;
	static void SetupViewPort() noexcept;
	static void ClearRenderTarget() noexcept;
	static void PresentRenderTargets() noexcept;

	static GraphicAdapterInfo* S_pGraphicDevices;
	static unsigned short S_GraphicDevices_SIZE;
private:
	static Window* S_pWindow;

	static ID3D11Device* S_pDevice;
	static ID3D11DeviceContext* S_pDeviceContext;
	static IDXGISwapChain* S_pSwapChain;
	static ID3D11RenderTargetView* S_pRenderTargetView;
	static ID3D11DepthStencilView* S_pDepthStencilView;
};


