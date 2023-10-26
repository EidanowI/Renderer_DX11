#include "../../Headers/DX11_Base.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")



Graphics::GraphicAdapterInfo* Graphics::S_pGraphicDevices = nullptr;
unsigned short Graphics::S_graphicDevices_SIZE = 0u;

Window* Graphics::S_pWindow = nullptr;
ID3D11Device* Graphics::S_pDevice = nullptr;
ID3D11DeviceContext* Graphics::S_pDeviceContext = nullptr;
IDXGISwapChain* Graphics::S_pSwapChain = nullptr;
ID3D11RenderTargetView* Graphics::S_pRenderTargetView = nullptr;
ID3D11DepthStencilView* Graphics::S_pDepthStencilView = nullptr;

void Graphics::GetDevices(const std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters, Graphics::GraphicAdapterInfo* adapterInfos) noexcept {
	int bestScore = 0;
	int bestScoreId = 0;

	DXGI_ADAPTER_DESC desc{};
	for (int i = 0; i < adapters.size(); i++) {
		int score = 0;
		adapters[i]->GetDesc(&desc);
		std::wstring description(desc.Description);

		if (description.find(L"RTX") != std::wstring::npos || description.find(L"GTX") != std::wstring::npos || description.find(L"RX") != std::wstring::npos) {
			score += 5;
		}
		score += (int)(desc.DedicatedVideoMemory / 1000000000);


		if (score > bestScore) {
			bestScore = score;
			bestScoreId = i;
		}
		adapterInfos[i] = {desc.DeviceId,  std::wstring(desc.Description), score};
	}
}

IDXGIAdapter* Graphics::GetMostPowerfulAdapter(const std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters, Graphics::GraphicAdapterInfo* adapterInfos) noexcept {
	int bestScore = 0;
	int bestScoreId = 0;

	DXGI_ADAPTER_DESC desc{};
	for (int i = 0; i < adapters.size(); i++) {
		int score = 0;
		adapters[i]->GetDesc(&desc);
		std::wstring description(desc.Description);

		if (description.find(L"RTX") != std::wstring::npos || description.find(L"GTX") != std::wstring::npos || description.find(L"RX") != std::wstring::npos) {
			score += 5;
		}
		score += (int)(desc.DedicatedVideoMemory / 1000000000);


		if (score > bestScore) {
			bestScore = score;
			bestScoreId = i;
		}
		//adapterInfos[i] = {desc.DeviceId,  std::wstring(desc.Description), score};
	}


#ifndef NDEBUG 
	adapters[bestScoreId].Get()->GetDesc(&desc);
	OutputDebugString(L"\n\nVIDEO DEVICE DESCRIPTION\n");
	OutputDebugString(desc.Description);
	OutputDebugString(L"\n\n");
#endif // !1
	adapters[bestScoreId]->GetDesc(&desc);

	Window::S_CreateWindowParams.deviceID = desc.DeviceId;
	return adapters[bestScoreId].Get();
}

IDXGIAdapter* Graphics::GetAdapterByID(const std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters) noexcept {
	DXGI_ADAPTER_DESC desc{};
	for (int i = 0; i < adapters.size(); i++) {
		adapters[i]->GetDesc(&desc);
		std::wstring description(desc.Description);
		if (desc.DeviceId == Window::S_CreateWindowParams.deviceID) {
			return adapters[i].Get();
		}

	}
}


void Graphics::Initialize(HWND hWnd, const void* pWindow) noexcept {
	if (S_pWindow != nullptr) return;

	int hres = 0;
	S_pWindow = (Window*)pWindow;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	ERROR_HRESULT(CreateDXGIFactory(__uuidof(IDXGIFactory), &pFactory));
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters;
	Microsoft::WRL::ComPtr<IDXGIAdapter> tmp_pAdapter;
	for (unsigned int i = 0; pFactory->EnumAdapters(i, &tmp_pAdapter) == S_OK; i++) {
		adapters.push_back(tmp_pAdapter);
	}

	S_graphicDevices_SIZE = adapters.size();
	S_pGraphicDevices = new Graphics::GraphicAdapterInfo[S_graphicDevices_SIZE];

	DXGI_SWAP_CHAIN_DESC swDesc{};
	{
		swDesc.BufferDesc.Width = 0;//0 - means get it from window
		swDesc.BufferDesc.Height = 0;
		swDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swDesc.BufferDesc.RefreshRate.Numerator = 0;
		swDesc.BufferDesc.RefreshRate.Denominator = 0;
		swDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swDesc.SampleDesc.Count = 1u;
		swDesc.SampleDesc.Quality = 0u;

		swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swDesc.BufferCount = 1;//So we want one back buffer (double buffering one front and one back)
		swDesc.OutputWindow = hWnd;
		swDesc.Windowed = TRUE;
		swDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//vanila (gives you the best perfomance at many cases
		swDesc.Flags = 0;
	}

	GetDevices(adapters, S_pGraphicDevices);
	IDXGIAdapter* pAdapter = nullptr;
	if (S_pWindow->S_CreateWindowParams.deviceID == -1) {
		pAdapter = GetMostPowerfulAdapter(adapters, S_pGraphicDevices);
	}
	else {
		pAdapter = GetAdapterByID(adapters);
	}
	ERROR_HRESULT(D3D11CreateDeviceAndSwapChain(
		pAdapter,//video adapter device 0 - use system setings
		D3D_DRIVER_TYPE_UNKNOWN,// Driver.
		nullptr,//handle to dll with software driver
		DX_DEBUG_LAYER,//layers
		nullptr,//------features
		0,//features count
		D3D11_SDK_VERSION,
		&swDesc,//swap chain description (input)
		&S_pSwapChain,// (output)
		&S_pDevice,// (output)
		nullptr,// feature level (output)
		&S_pDeviceContext// (output)//
	));


	//get access to texture subresource in swap chain (back buffer)
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	ERROR_HRESULT(S_pSwapChain->GetBuffer(0/*0 - is a back buffer*/, __uuidof(ID3D11Resource), &pBackBuffer));
	ERROR_HRESULT(S_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &S_pRenderTargetView));//Set this parameter to NULL to create a view that accesses all of the subresources in mipmap level 0.

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	D3D11_DEPTH_STENCIL_DESC dptStnDesc{};
	{
		dptStnDesc.DepthEnable = TRUE;
		dptStnDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dptStnDesc.DepthFunc = D3D11_COMPARISON_LESS;
		//dptStnDesc.StencilEnable = FALSE;
		//dptStnDesc.StencilReadMask = NULL;
		//dptStnDesc.StencilWriteMask = NULL;
		//dptStnDesc.FrontFace = D3D11_DEPTH_STENCILOP_DESC{};
		//dptStnDesc.BackFace = D3D11_DEPTH_STENCILOP_DESC{};
	}

	S_pDevice->CreateDepthStencilState(&dptStnDesc, &pDepthStencilState);
	S_pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture2D;
	D3D11_TEXTURE2D_DESC dpthStnTextureDesc{};
	{
		dpthStnTextureDesc.Width = S_pWindow->S_WindowWidth;
		dpthStnTextureDesc.Height = S_pWindow->S_WindowHeight;
		dpthStnTextureDesc.MipLevels = 1u;
		dpthStnTextureDesc.ArraySize = 1u;
		dpthStnTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dpthStnTextureDesc.SampleDesc.Count = 1u;
		dpthStnTextureDesc.SampleDesc.Quality = 0u;
		dpthStnTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		dpthStnTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//dpthStnTextureDesc.CPUAccessFlags = 0u;
		//dpthStnTextureDesc.MiscFlags = 0u;
	}
	S_pDevice->CreateTexture2D(&dpthStnTextureDesc, nullptr, &pDepthStencilTexture2D);


	D3D11_DEPTH_STENCIL_VIEW_DESC dpthStncViewDesc{};
	{
		dpthStncViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dpthStncViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dpthStncViewDesc.Texture2D.MipSlice = 0u;
	}
	S_pDevice->CreateDepthStencilView(pDepthStencilTexture2D.Get(), &dpthStncViewDesc, &S_pDepthStencilView);

	SetupViewPort();
	SetRenderTargets();

	ImGui_ImplDX11_Init(S_pDevice, S_pDeviceContext);

	/*{
		IDXGIDevice* pDevice1 = nullptr;
		m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice1);
		IDXGIAdapter* pAdapter1 = nullptr;
		pDevice1->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter1);
		IDXGIFactory* pfactory1 = nullptr;
		pAdapter1->GetParent(__uuidof(IDXGIFactory), (void**)&pfactory1);
		DXGI_ADAPTER_DESC desk1{};
		pAdapter1->GetDesc(&desk1);
		OutputDebugString((std::to_wstring(desk1.VendorId) + L" ---- VendorId\n\n\n").c_str());
		pfactory1->Release();
		pAdapter1->Release();
		pDevice1->Release();
	}*/
}

void Graphics::UnInitialize() noexcept {
	if (S_pWindow == nullptr) return;

	if(S_pGraphicDevices != nullptr)
	{
		S_graphicDevices_SIZE = 0;
		delete[] S_pGraphicDevices;
		S_pGraphicDevices = nullptr;
	}

	S_pWindow = nullptr;
	ImGui_ImplDX11_Shutdown();

	if (S_pDepthStencilView != nullptr) {
		S_pDepthStencilView->Release();
		S_pDepthStencilView = nullptr;
	}
	if (S_pRenderTargetView != nullptr) {
		S_pRenderTargetView->Release();
		S_pRenderTargetView = nullptr;
	}
	if (S_pSwapChain != nullptr) {
		S_pSwapChain->Release();
		S_pSwapChain = nullptr;
	}
	if (S_pDeviceContext != nullptr) {
		S_pDeviceContext->Release();
		S_pDeviceContext = nullptr;
	}
	if (S_pDevice != nullptr) {
		S_pDevice->Release();
		S_pDevice = nullptr;
	}


}


void Graphics::SetRenderTargets() noexcept {
	S_pDeviceContext->OMSetRenderTargets(1u, &S_pRenderTargetView, S_pDepthStencilView);//sprecify stensilDepth
}

void Graphics::SetupViewPort() noexcept {
	D3D11_VIEWPORT viewPort{};
	{
		viewPort.TopLeftX = 0.f;
		viewPort.TopLeftY = 0.f;
		viewPort.Width = S_pWindow->S_WindowWidth;
		viewPort.Height = S_pWindow->S_WindowHeight;
		viewPort.MinDepth = 0.f;
		viewPort.MaxDepth = 1.f;
	}
	S_pDeviceContext->RSSetViewports(1, &viewPort);//WE need a viewport for rasterization
}

void Graphics::ClearRenderTarget() noexcept {
	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	S_pDeviceContext->ClearRenderTargetView(S_pRenderTargetView, color);
	S_pDeviceContext->ClearDepthStencilView(S_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
}

void Graphics::PresentRenderTargets() noexcept {
	ERROR_PRESENT(S_pSwapChain->Present(1u, 0u));
}