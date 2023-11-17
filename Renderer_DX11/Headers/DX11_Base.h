#pragma once
#include <Windows.h>
#include <wrl.h>//for Microsoft::WRL
#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_impl_win32.h"
#include "../Dependencies/imGUI/imgui_impl_dx11.h"

#include "../Dependencies/PNGLoader/lodepng.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <string>
#include <optional>
#include <vector>
#include <bitset>
#include <fstream>

#include "DX11_ErrorMacro.h"
#include "GparhicsFundamentals.h"


extern bool SHOULD_CLOSE_WINDOW_AND_CREATE_NEW;

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

/// <summary>
/// Used for tracking user input. Uses function pointers to quickly replace button functionality and optimizations.
/// </summary>
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

public:
	static int S_currentCursorPosX;
	static int S_currentCursorPosY;
	static int S_cursorDeltaX;
	static int S_cursorDeltaY;

	static bool S_isCursorInClientArea;
	static bool S_isCursorEnable;
	static int S_wheelDelta;

	static std::vector<unsigned char> S_rawBuffer;
};

/// <summary>
/// GUI for convenient developing and debugging
/// </summary>
class ImGUIManager {
public:
	ImGUIManager() noexcept;
	~ImGUIManager() noexcept;

	static void NewFrame() noexcept;
	static void Render() noexcept;
	static void ShowGUIWindows() noexcept;

	static void EnableImGuiMouse();
	static void DisableImGuiMouse();

private:
	static bool S_isCreated;
	static bool S_isShowGraphicsSetupWindow;
};

class GameObject {
public:
	void SetPosition(const GraphicsFundament::Vector3D& position) noexcept {
		m_position = position;
		UpdateTransform();
	}
	void SetPosition(float x, float y, float z) noexcept {
		m_position = GraphicsFundament::Vector3D(x, y, z);
		UpdateTransform();
	}
	GraphicsFundament::Vector3D GetPosition() const noexcept {
		return m_position;
	}
	void SetRotation(const GraphicsFundament::Vector3D& rotation) noexcept {
		m_rotation = rotation;
		UpdateTransform();
	}
	void SetRotation(float x, float y, float z) noexcept {
		m_rotation = GraphicsFundament::Vector3D(x, y, z);;
		UpdateTransform();
	}
	GraphicsFundament::Vector3D GetRotation() const noexcept {
		return m_rotation;
	}
	void SetScale(const GraphicsFundament::Vector3D& scale) noexcept {
		m_scale = scale;
		UpdateTransform();
	}
	void SetScale(float x, float y, float z) noexcept {
		m_scale = GraphicsFundament::Vector3D(x, y, z);;
		UpdateTransform();
	}
	GraphicsFundament::Vector3D GetScale() const noexcept {
		return m_scale;
	}

protected:
	virtual void UpdateConstBuffer() noexcept {}
	void UpdateTransform() noexcept {
		m_transform = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
			DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		UpdateConstBuffer();
	}

protected:
	DirectX::XMMATRIX m_transform = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

	GraphicsFundament::Vector3D m_position = GraphicsFundament::Vector3D();
	GraphicsFundament::Vector3D m_rotation = GraphicsFundament::Vector3D();
	GraphicsFundament::Vector3D m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
};

class Camera : public GameObject {
public:
	DirectX::XMMATRIX m_camOrientation;
	GraphicsFundament::Vector3D m_right;
	GraphicsFundament::Vector3D m_up;
	GraphicsFundament::Vector3D m_forward;
	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_projection;

	Camera(const GraphicsFundament::Vector3D& pos, const GraphicsFundament::Vector3D& rotation) noexcept;

	GraphicsFundament::Vector3D GetRight() const noexcept {
		return m_right;
	}
	GraphicsFundament::Vector3D GetUp() const noexcept {
		return m_up;
	}
	GraphicsFundament::Vector3D GetForward() const noexcept {
		return m_forward;
	}

	void UpdateVectors() noexcept;

	void SetCameraFPSRotation(float x, float y, float z) noexcept;
	void TranslatePosition(float a, GraphicsFundament::Vector3D& direction) noexcept;
};

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

	static void AddCameraAndSetActive(const Camera& camera) noexcept {
		S_activeCameraIndex = S_cameras.size();
		S_cameras.push_back(camera);
	}
	static Camera& GetActiveCamera() noexcept{
		return S_cameras[S_activeCameraIndex];
	}

	struct VertCBuf1Struct_TestVert {
		DirectX::XMMATRIX modelMatrix;
		DirectX::XMMATRIX normalMatrix;
	};
	template<class T>
	static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateConstBuffer(const T& constBufferStruct) {
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;

		D3D11_BUFFER_DESC constBuffDesc{};
		{
			constBuffDesc.ByteWidth = sizeof(T);
			constBuffDesc.Usage = D3D11_USAGE_DYNAMIC;// D3D11_USAGE_IMMUTABLE can gives more fps
			constBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//
			constBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constBuffDesc.MiscFlags = 0u;// miscellaneous
			constBuffDesc.StructureByteStride = 0u;
		}

		D3D11_SUBRESOURCE_DATA constBuffSubResData{};
		{
			constBuffSubResData.pSysMem = (void*)&constBufferStruct;
		}

		Graphics::GetDevice()->CreateBuffer(&constBuffDesc, &constBuffSubResData, &pConstBuffer);

		return pConstBuffer;
	}
	template<class T>
	static void UpdateConstBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& pConstBuff, const T& constBufferStruct) {
		D3D11_MAPPED_SUBRESOURCE constBuffMappedSRes;
		Graphics::GetDeviceContext()->Map(pConstBuff.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &constBuffMappedSRes);
		memcpy(constBuffMappedSRes.pData, &constBufferStruct, sizeof(T));
		Graphics::GetDeviceContext()->Unmap(pConstBuff.Get(), 0u);
	}

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

/// <summary>
/// Each shader(file with compiled HLSL code) is named in the format Name.cso, where name is an array of 8 elements(For example: TestVets.cso, HaHaVERT.cso and etc. 
/// This array is cast to a unique ID, which is a big number (long long), used to determine whether the shader has been loaded into the ïêôçðøñû card or needs to be loaded from disk.
/// </summary>
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


/// <summary>
/// Used as a conteiner for storing images.
/// </summary>
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

template<class Vertex>
class Mesh {
public:
	Mesh() noexcept
		:
		m_vertexShaderIndex(0u),
		m_pixelShaderIndex(0u),
		m_pVertexBuffer(nullptr),
		m_pIndexBuffer(nullptr),
		m_pInputLayout(nullptr),
		m_pSourceVerticies(nullptr),
		m_sourceVerticiesNum(0u),
		m_pSourceIndecies(nullptr),
		m_sourceIndeciesNum(0u)
	{}
	Mesh(Vertex* verticies, const int verticiesNums, TrianglePoly* indecies, int indeciesNum, const char* vertexUID, const char* pixelUID) noexcept
		:
		m_pSourceVerticies(verticies), m_sourceVerticiesNum(verticiesNums), m_pSourceIndecies(indecies), m_sourceIndeciesNum(indeciesNum)
	{
		int hres = 0;
		D3D11_BUFFER_DESC verticiesBufferDesc{};
		{
			verticiesBufferDesc.ByteWidth = m_sourceVerticiesNum * sizeof(Vertex);
			verticiesBufferDesc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_IMMUTABLE can gives more fps
			verticiesBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//
			verticiesBufferDesc.CPUAccessFlags = 0u;
			verticiesBufferDesc.MiscFlags = 0u;// miscellaneous
			verticiesBufferDesc.StructureByteStride = sizeof(Vertex);
		}
		D3D11_SUBRESOURCE_DATA verticiesSubResData{};
		{
			verticiesSubResData.pSysMem = (void*)m_pSourceVerticies;
		}
		ERROR_HRESULT(Graphics::GetDevice()->CreateBuffer(&verticiesBufferDesc, &verticiesSubResData, &m_pVertexBuffer));

		D3D11_BUFFER_DESC indeciesBufferDesc{};
		{
			indeciesBufferDesc.ByteWidth = m_sourceIndeciesNum * sizeof(TrianglePoly);
			indeciesBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indeciesBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indeciesBufferDesc.CPUAccessFlags = 0u;
			indeciesBufferDesc.MiscFlags = 0u;
			indeciesBufferDesc.StructureByteStride = sizeof(unsigned short);
		}
		D3D11_SUBRESOURCE_DATA indeciesSubData{};
		{
			indeciesSubData.pSysMem = (void*)m_pSourceIndecies;
		}
		ERROR_HRESULT(Graphics::GetDevice()->CreateBuffer(&indeciesBufferDesc, &indeciesSubData, &m_pIndexBuffer));

		m_vertexShaderIndex = ShaderSystem::GetVertexShaderIndex(vertexUID, "Shaders/");
		m_pixelShaderIndex = ShaderSystem::GetPixelShaderIndex(pixelUID, "Shaders/");


		ERROR_HRESULT(Graphics::GetDevice()->CreateInputLayout(Vertex::inputElementDesc,
			Vertex::elementDescNum,
			ShaderSystem::S_VertexShaders[m_vertexShaderIndex].shaderSourceCode.data(),
			ShaderSystem::S_VertexShaders[m_vertexShaderIndex].shaderSourceCode.size(),
			&m_pInputLayout));
		//m_pInputLayout = Vertex::CreateInputLayout(m_vertexShaderIndex);
	}
	~Mesh() noexcept {
		if (m_pVertexBuffer != nullptr) {
			m_pVertexBuffer->Release();
			m_pVertexBuffer = nullptr;
		}
		if (m_pIndexBuffer != nullptr) {
			m_pIndexBuffer->Release();
			m_pIndexBuffer = nullptr;
		}
		if (m_pInputLayout != nullptr) {
			m_pInputLayout->Release();
			m_pInputLayout = nullptr;
		}

		if (m_pSourceVerticies != nullptr) {
			delete[] m_pSourceVerticies;
			m_pSourceVerticies = nullptr;
		}
		if (m_pSourceIndecies != nullptr) {
			delete[] m_pSourceIndecies;
			m_pSourceIndecies = nullptr;
		}
	}

	void Bind() noexcept {
		unsigned int strides[] = { sizeof(Vertex) };
		unsigned int offsets[] = { 0u };
		Graphics::GetDeviceContext()->IASetVertexBuffers(0u, 1u, &m_pVertexBuffer, strides, offsets);
		Graphics::GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
		Graphics::GetDeviceContext()->IASetInputLayout(m_pInputLayout);
		Graphics::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Graphics::GetDeviceContext()->VSSetShader(ShaderSystem::S_VertexShaders[m_vertexShaderIndex].vertexShader.Get(), nullptr, 0u);
		Graphics::GetDeviceContext()->PSSetShader(ShaderSystem::S_PixelShaders[m_pixelShaderIndex].pixelShader.Get(), nullptr, 0u);
	}
	void Draw() noexcept {
		Graphics::GetDeviceContext()->DrawIndexed(m_sourceIndeciesNum * 3u, 0u, 0u);
	}

private:

	unsigned short m_vertexShaderIndex;
	unsigned short m_pixelShaderIndex;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pInputLayout;

	Vertex* m_pSourceVerticies;
	unsigned int m_sourceVerticiesNum;
	TrianglePoly* m_pSourceIndecies;
	unsigned int m_sourceIndeciesNum;
};

class Cube : public GameObject {
public:
	Cube() noexcept;
	~Cube() noexcept;

	void UpdateConstBuffer() noexcept override;

	void Bind() noexcept;
	void Draw() noexcept;


	Graphics::VertCBuf1Struct_TestVert m_vertConstBuf1Struct;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertConstBuf1;
	static Mesh<Vertex48B>* S_pCubeMesh;
	static unsigned int S_initializedNum;
};

