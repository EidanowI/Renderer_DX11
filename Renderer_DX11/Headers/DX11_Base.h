#pragma once
#include <Windows.h>
#include <wrl.h>//for Microsoft::WRL
#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_impl_win32.h"
#include "../Dependencies/imGUI/imgui_impl_dx11.h"

#include "../Dependencies/PNGLoader/lodepng.h"

#include <d3d11.h>

#include <string>
#include <optional>
#include <vector>
#include <bitset>
#include <fstream>

#include "DX11_ErrorMacro.h"


extern bool SHOULD_CLOSE_WINDOW_AND_CREATE_NEW;
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
class InputSystem {
	friend class Window;
	friend LRESULT WindowProcess(HWND, UINT, WPARAM, LPARAM);
	friend void STANDARTMOVE(int x, int y) noexcept;

public:
	enum MouseButton {
		LEFT_MOUSE_BUTON = 0,
		RIGHT_MOUSE_BUTTON = 1,
		MIDDLE_MOUSE_BUTTON = 2,
		XBUTTON1_MOUSE_BUTTON = 3,
		XBUTTON2_MOUSE_BUTTON = 4,


		THIS_USED_ONLY_FOR_SETTING_SIZE
	};
private:
	struct MousePressedInfo {
		WPARAM wParam;
		int x;
		int y;
	};
public:
	static int Initialize() noexcept;
	static void UnInitialize() noexcept;
	static void UpdateInput() noexcept;

	static bool GetIsCursorInCliendArea() noexcept {
		return S_isCursorInClientArea;
	}
	static void SetIsCursorInClientArea(bool isCinCA) noexcept {
		S_isCursorInClientArea = isCinCA;
	}

	static void AddPressedKey(unsigned char keyCode, LPARAM lParam) noexcept;
	static void RemovePressedKey(unsigned char keyCode) noexcept;

	static void AddPressedMButton(unsigned char mButton, WPARAM wParam, LPARAM lParam) noexcept;
	static void RemovePressedMButton(unsigned char mButton) noexcept;

	static void KillFocus() noexcept;

	static void (**S_ppFunction_ONCE_KEY)(LPARAM lParam) noexcept;
	static void (**S_ppFunction_KEY_ISPRESSED)(LPARAM lParam) noexcept;
	static void (**S_ppFunction_KEY_RELEASE)(LPARAM lParam) noexcept;
	static void (*S_pFunction_ONCHAR)(WPARAM wParam, LPARAM lParam) noexcept;

	static void (**S_ppFunction_ONCE_MBUTTON)(WPARAM wParam, int x, int y) noexcept;
	static void (**S_ppFunction_MOUSE_ISPRESSED)(WPARAM wParam, int x, int y) noexcept;
	static void (**S_ppFunction_MOUSE_RELEASE)(WPARAM wParam, int x, int y) noexcept;
	static void (*S_pFunction_MOVE_MOUSE)(int x, int y) noexcept;

	static void (*S_pFunction_WHEELUP_STEP)(int x, int y) noexcept;
	static void (*S_pFunction_WHEELDOWN_STEP)(int x, int y) noexcept;

	static void(*S_pFunction_OnCursorEnterCA)() noexcept;
	static void(*S_pFunction_OnCursorLeaveCA)() noexcept;

private:
	static unsigned char* S_pPressedKeyCodes;
	static unsigned short S_pressedKeyCount;
	static unsigned char* S_pFromKeyCodesToIndex;
	static std::bitset<256> S_isKeysPressed;
	static LPARAM* S_pKeysLParams;

	static unsigned char* S_pPressedMButtons;
	static unsigned char S_pressedMButtonsCount;
	static unsigned char* S_pFromMButtonsToIndex;
	static std::bitset<THIS_USED_ONLY_FOR_SETTING_SIZE> S_isMouseBPressed;
	static MousePressedInfo* S_pMButtonsInfos;

	static int S_currentCursorPosX;
	static int S_currentCursorPosY;
	static int S_cursorDeltaX;
	static int S_cursorDeltaY;

	static bool S_isCursorInClientArea;
	static int S_wheelDelta;
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
	static unsigned short S_graphicDevices_SIZE;
private:
	static Window* S_pWindow;

	static ID3D11Device* S_pDevice;
	static ID3D11DeviceContext* S_pDeviceContext;
	static IDXGISwapChain* S_pSwapChain;
	static ID3D11RenderTargetView* S_pRenderTargetView;
	static ID3D11DepthStencilView* S_pDepthStencilView;
};
class ShaderSystem {
	struct VertexShader {
		long long shader_UID;
		std::vector<char> shaderSourceCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	};
	struct PixelShader {
		long long shader_UID;
		std::vector<char> shaderSourceCode;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	};
public:

	static unsigned int GetVertexShaderIndex(const char* shader_UID, const std::string& path) noexcept;
	static unsigned int GetPixelShaderIndex(const char* shader_UID, const std::string& path) noexcept;
	static void Clear() noexcept {
		S_VertexShaders = std::vector<VertexShader>();
		S_PixelShaders = std::vector<PixelShader>();
	}

	static std::vector<VertexShader> S_VertexShaders;
	static std::vector<PixelShader> S_PixelShaders;
};
class ImageSurface {
public:
	struct Color {
		unsigned int color;
		Color() noexcept : color(0) {}
		Color(const Color& color) noexcept : color(color.color) {}
		Color(unsigned int color) noexcept : color(color) {}
		typedef unsigned char UCHAR;
		Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a)noexcept : color(((r << 24u) | (g << 16u) | (b << 8u) | a)) {}
		UCHAR GetR() const { return color >> 24u; }
		UCHAR GetG() const { return color >> 16u; }
		UCHAR GetB() const { return color >> 8u; }
		UCHAR GetA() const { return color; }
		void SetR(const UCHAR r) noexcept { (color &= 0x00FFFFFF) |= (r << 24u); }
		void SetG(const UCHAR g) noexcept { (color &= 0xFF00FFFF) |= (g << 16u); }
		void SetB(const UCHAR b) noexcept { (color &= 0xFFFF00FF) |= (b << 8u); }
		void SetA(const UCHAR a) noexcept { (color &= 0xFFFFFF00) |= a; }
	};
	ImageSurface(unsigned int width, unsigned int height, std::vector<Color>& bufer) noexcept;
	ImageSurface(unsigned int width, unsigned int height) noexcept;
	ImageSurface(const ImageSurface& surface) = delete;
	~ImageSurface() noexcept;
	void ClearSurface(const Color& color) noexcept;
	Color& GetPixel(unsigned int x, unsigned int y) const noexcept;
	void SetPixel(unsigned int x, unsigned int y, const Color color) noexcept;
	char* GetBuffer() const noexcept {
		return (char*)m_buffer.data();
	}
	unsigned int GetWidth() const noexcept {
		return m_width;
	}
	unsigned int GetHeight() const noexcept {
		return m_height;
	}
	static ImageSurface FromFile(const std::string& name) noexcept;
	void Copy(const ImageSurface& src) noexcept;
private:
	std::vector<Color> m_buffer;
	unsigned int m_width;
	unsigned int m_height;
};
class BindToPipeline {
public:
	virtual void Bind() noexcept = 0;
};
class Sampler : public BindToPipeline {
public:
	Sampler() noexcept;
	void Bind() noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleState;
};
class Texture : public BindToPipeline {
public:
	Texture(const ImageSurface& surface, unsigned int index = 0u) noexcept;
	void Bind() noexcept override;
private:
	unsigned int m_textureBindIndex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSahderResView;
};



