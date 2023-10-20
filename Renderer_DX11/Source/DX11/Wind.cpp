#include "../../Headers/DX11_Base.h"
#include "../../resource.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::WindowClass Window::WindowClass::S_wndClass = Window::WindowClass();
const wchar_t* Window::WindowClass::S_wndClassName = L"D3X11_Renderer";
unsigned short Window::S_WindowWidth;
unsigned short Window::S_WindowHeight;
HWND Window::S_hWnd = nullptr;
Window::CreateWindowParams Window::S_CreateWindowParams = {-1};


Window::WindowClass::WindowClass() noexcept : m_hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WindowProcess;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = (HICON)LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0);
	//wc.hIcon = (HICON)0;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = (HICON)LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
	//wc.hIconSm = (HICON)0;
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass() noexcept
{
	UnregisterClass(S_wndClassName, GetInstance());
}

Window::Window(unsigned int width, unsigned int height, const wchar_t* windowName) noexcept{
	if (S_hWnd != nullptr) return;

	S_WindowWidth = width;
	S_WindowHeight = height;

	//should to use for correct window size
	RECT rect;
	rect.left = 100;
	rect.right = rect.left + width;
	rect.top = 100;
	rect.bottom = rect.top + height;

	ERROR_BOOL(AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE));

	S_hWnd = CreateWindow(
		WindowClass::GetName(), windowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	ERROR_IF(S_hWnd == nullptr, L"NULL POINTER");
	ShowWindow(S_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(S_hWnd);

	ImGui_ImplWin32_Init(S_hWnd);
}
Window::~Window() noexcept
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(S_hWnd);
	S_hWnd = nullptr;
}

void Window::SetWindowTitle(const std::wstring& title) noexcept {
	SetWindowText(S_hWnd, title.c_str());;
}
std::optional<int> Window::ProcessMsg() noexcept {
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return msg.wParam;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return std::optional<int>{};
}

LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}