#pragma once

#include <wrl.h>
#include <d3d11.h>

#include <string>
#include <vector>

class Window;
class Camera;

/// <summary>
/// Used for obtaining and selecting a graphics adaptor, driver. Encapsulates DirectX11 stuff.
/// </summary>
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

	static ID3D11Device* GetDevice() noexcept;
	static ID3D11DeviceContext* GetDeviceContext() noexcept;

	static void SetRenderTargets() noexcept;
	static void SetupViewPort() noexcept;
	static void ClearRenderTarget() noexcept;
	static void PresentRenderTargets() noexcept;

	static void AddCameraAndSetActive(const Camera& camera) noexcept;
	static Camera& GetActiveCamera() noexcept;

	static ID3D11Buffer* CreateConstBuffer(const char* pConstBufferStruct, unsigned int sizeOfStruct) noexcept;
	static void UpdateConstBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& pConstBuff, void* pConstBufferStruct, int sizeOfStruct) noexcept;

	static GraphicAdapterInfo* S_pGraphicDevices;
	static unsigned short S_graphicDevices_SIZE;

private:
	static std::vector<Camera> S_cameras;/// all available cameras
	static int S_activeCameraIndex;

	static Window* S_pWindow;

	static ID3D11Device* S_pDevice;
	static ID3D11DeviceContext* S_pDeviceContext;
	static IDXGISwapChain* S_pSwapChain;
	static ID3D11RenderTargetView* S_pRenderTargetView;
	static ID3D11DepthStencilView* S_pDepthStencilView;
};
