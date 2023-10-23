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
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0b01000000000000000000000000000000)) {//30 bit is 1 if key was pressed before this message
			InputSystem::S_ppFunction_ONCE_KEY[(unsigned char)wParam](lParam);
			InputSystem::AddPressedKey((unsigned char)wParam, lParam);
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		InputSystem::S_ppFunction_KEY_RELEASE[(unsigned char)wParam](lParam);
		InputSystem::RemovePressedKey((unsigned char)wParam);
		break;
	case WM_CHAR:
		InputSystem::S_pFunction_ONCHAR(wParam, lParam);
		break;
	case WM_KILLFOCUS:
		InputSystem::KillFocus();
		break;

	case WM_MOUSEMOVE:
	{
		short posX = *((short*)(char*)&lParam);
		short posY = *((short*)((char*)&lParam + 2));
		if (posX >= 0 && posX < Window::S_WindowWidth && posY >= 0 && posY < Window::S_WindowHeight)
		{

			//mouse.OnMouseMove(posX, posY);
			InputSystem::S_pFunction_MOVE_MOUSE(posX, posY);


			if (!InputSystem::GetIsCursorInCliendArea())
			{
				SetCapture(hWnd);
				InputSystem::SetIsCursorInClientArea(true);
				InputSystem::S_pFunction_OnCursorEnterCA();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				InputSystem::S_pFunction_MOVE_MOUSE(posX, posY);
			}
			else
			{
				ReleaseCapture();
				InputSystem::KillFocus();
				InputSystem::SetIsCursorInClientArea(false);
				InputSystem::S_pFunction_OnCursorLeaveCA();
			}
		}
	}
	break;
	case WM_LBUTTONDOWN:
		//if (wParam == (long long)0b00000101) MessageBox(nullptr, L"LINE: ", L"afaf", MB_OK | MB_ICONEXCLAMATION);
		InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::LEFT_MOUSE_BUTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::LEFT_MOUSE_BUTON, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::LEFT_MOUSE_BUTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::LEFT_MOUSE_BUTON);
		break;
	case WM_RBUTTONDOWN:
		InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::RIGHT_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::RIGHT_MOUSE_BUTTON, wParam, lParam);
		break;
	case WM_RBUTTONUP:
		InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::RIGHT_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::RIGHT_MOUSE_BUTTON);
		break;
	case WM_MBUTTONDOWN:
		InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::MIDDLE_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::MIDDLE_MOUSE_BUTTON, wParam, lParam);
		break;
	case WM_MBUTTONUP:
		InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::MIDDLE_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::MIDDLE_MOUSE_BUTTON);
		break;
	case WM_XBUTTONDOWN:
		if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON1) {
			InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::XBUTTON1_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::AddPressedMButton(InputSystem::XBUTTON1_MOUSE_BUTTON, wParam, lParam);
		}
		else if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON2) {
			InputSystem::S_ppFunction_ONCE_MBUTTON[InputSystem::XBUTTON2_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::AddPressedMButton(InputSystem::XBUTTON2_MOUSE_BUTTON, wParam, lParam);
		}
		break;
	case WM_XBUTTONUP:
		if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON1) {
			InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::XBUTTON1_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::RemovePressedMButton(InputSystem::XBUTTON1_MOUSE_BUTTON);
		}
		else if (*(unsigned short*)(((char*)&wParam) + 2) & XBUTTON2) {
			InputSystem::S_ppFunction_MOUSE_RELEASE[InputSystem::XBUTTON2_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
			InputSystem::RemovePressedMButton(InputSystem::XBUTTON2_MOUSE_BUTTON);
		}
		break;
	case WM_MOUSEWHEEL:
		//if (*(short*)&wParam != (short)0b00000100) break; -- BREAK IF YOU DOESNT HOLD SHIFT WHILE SPIN THE WHEEL
		//InputSystem::func_WHEELMOVE(wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::S_wheelDelta += (short)*((char*)&wParam + 2);
		while (InputSystem::S_wheelDelta >= WHEEL_DELTA) {
			InputSystem::S_wheelDelta -= WHEEL_DELTA;
			InputSystem::S_pFunction_WHEELUP_STEP(*((short*)(void*)&lParam), *((short*)((char*)&lParam + 2)));
		}
		while (InputSystem::S_wheelDelta <= -WHEEL_DELTA) {
			InputSystem::S_wheelDelta += WHEEL_DELTA;
			InputSystem::S_pFunction_WHEELDOWN_STEP(*((short*)(void*)&lParam), *((short*)((char*)&lParam + 2)));
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}