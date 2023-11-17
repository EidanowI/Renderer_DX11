#include "../../Headers/DX11_Base.h"




extern float cameraXRotation;
extern float cameraYRotation;
extern float cameraZRotation;

void EMPTYFUNC() noexcept {};
void EMPTYFUNC(LPARAM lParam) noexcept {};
void EMPTYFUNC(WPARAM wParam, LPARAM lParam) noexcept {};
void EMPTYFUNC(WPARAM wParam, int x, int y) noexcept {};
void EMPTYFUNC(int x, int y) noexcept {};
void STANDART_MOVE(int x, int y) noexcept
{
	//InputSystem::S_cursorDeltaX = x - InputSystem::S_currentCursorPosX;
	//InputSystem::S_cursorDeltaY = y - InputSystem::S_currentCursorPosY;
	InputSystem::S_currentCursorPosX = x;
	InputSystem::S_currentCursorPosY = y;
}
void STANDART_ESCAPE(LPARAM lParam) noexcept
{
	if (InputSystem::S_isCursorEnable == true) {
		InputSystem::S_isCursorEnable = false;
		Window::DisableCursor();
		Window::FixateCursor();
		ImGUIManager::DisableImGuiMouse();
	}
	else {
		InputSystem::S_isCursorEnable = true;
		Window::EnableCursor();
		Window::FreeCursor();
		ImGUIManager::EnableImGuiMouse();
	}
}

unsigned char* InputSystem::S_pPressedKeyCodes = nullptr;
unsigned short InputSystem::S_pressedKeyCount = 0u;
unsigned char* InputSystem::S_pFromKeyCodesToIndex = nullptr;
std::bitset<256> InputSystem::S_isKeysPressed = std::bitset<256>();
LPARAM* InputSystem::S_pKeysLParams = nullptr;

unsigned char* InputSystem::S_pPressedMButtons = nullptr;
unsigned char InputSystem::S_pressedMButtonsCount = 0u;
unsigned char* InputSystem::S_pFromMButtonsToIndex = nullptr;
std::bitset<InputSystem::THIS_USED_ONLY_FOR_SETTING_SIZE> InputSystem::S_isMouseBPressed = std::bitset<InputSystem::THIS_USED_ONLY_FOR_SETTING_SIZE>();
InputSystem::MousePressedInfo* InputSystem::S_pMButtonsInfos = nullptr;

int InputSystem::S_currentCursorPosX = 0;
int InputSystem::S_currentCursorPosY = 0;
int InputSystem::S_cursorDeltaX = 0;
int InputSystem::S_cursorDeltaY = 0;

bool InputSystem::S_isCursorInClientArea = false;
bool InputSystem::S_isCursorEnable = true;
int InputSystem::S_wheelDelta = 0;
std::vector<unsigned char> InputSystem::S_rawBuffer = std::vector<unsigned char>();

void (**InputSystem::S_ppFunction_ONCE_KEY)(LPARAM lParam) noexcept = nullptr;
void (**InputSystem::S_ppFunction_KEY_ISPRESSED)(LPARAM lParam) noexcept = nullptr;
void (**InputSystem::S_ppFunction_KEY_RELEASE)(LPARAM lParam) noexcept = nullptr;
void (*InputSystem::S_pFunction_ONCHAR)(WPARAM wParam, LPARAM lParam) noexcept = nullptr;

void (**InputSystem::S_ppFunction_ONCE_MBUTTON)(WPARAM wParam, int x, int y) noexcept = nullptr;
void (**InputSystem::S_ppFunction_MOUSE_ISPRESSED)(WPARAM wParam, int x, int y) noexcept;
void (**InputSystem::S_ppFunction_MOUSE_RELEASE)(WPARAM wParam, int x, int y) noexcept;
void (*InputSystem::S_pFunction_MOVE_MOUSE)(int x, int y) noexcept;

void (*InputSystem::S_pFunction_WHEELUP_STEP)(int x, int y) noexcept = nullptr;
void (*InputSystem::S_pFunction_WHEELDOWN_STEP)(int x, int y) noexcept = nullptr;

void(*InputSystem::S_pFunction_OnCursorEnterCA)() noexcept = nullptr;
void(*InputSystem::S_pFunction_OnCursorLeaveCA)() noexcept = nullptr;


int InputSystem::Initialize() noexcept {
	S_pPressedKeyCodes = new unsigned char[256];
	S_pFromKeyCodesToIndex = new unsigned char[256];
	S_pKeysLParams = new LPARAM[256];

	S_pPressedMButtons = new unsigned char[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE];
	S_pFromMButtonsToIndex = new unsigned char[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE];
	S_pMButtonsInfos = new InputSystem::MousePressedInfo[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE];


	S_ppFunction_ONCE_KEY = new (void (*[256])(LPARAM lParam)noexcept);
	S_ppFunction_KEY_ISPRESSED = new (void (*[256])(LPARAM lParam)noexcept);
	S_ppFunction_KEY_RELEASE = new (void (*[256])(LPARAM lParam)noexcept);
	for (int i = 0; i < 256; i++) {
		S_ppFunction_ONCE_KEY[i] = EMPTYFUNC;
		S_ppFunction_KEY_ISPRESSED[i] = EMPTYFUNC;
		S_ppFunction_KEY_RELEASE[i] = EMPTYFUNC;
	}
	S_pFunction_ONCHAR = EMPTYFUNC;

	S_ppFunction_ONCE_MBUTTON = new (void(*[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE])(WPARAM wParam, int x, int y) noexcept);
	S_ppFunction_MOUSE_ISPRESSED = new (void(*[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE])(WPARAM wParam, int x, int y) noexcept);
	S_ppFunction_MOUSE_RELEASE = new (void(*[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE])(WPARAM wParam, int x, int y) noexcept);
	for (int i = 0; i < THIS_USED_ONLY_FOR_SETTING_SIZE; i++) {
		S_ppFunction_ONCE_MBUTTON[i] = EMPTYFUNC;
		S_ppFunction_MOUSE_ISPRESSED[i] = EMPTYFUNC;
		S_ppFunction_MOUSE_RELEASE[i] = EMPTYFUNC;
	}
	S_pFunction_MOVE_MOUSE = STANDART_MOVE;

	S_pFunction_WHEELUP_STEP = EMPTYFUNC;
	S_pFunction_WHEELDOWN_STEP = EMPTYFUNC;

	S_pFunction_OnCursorEnterCA = EMPTYFUNC;
	S_pFunction_OnCursorLeaveCA = EMPTYFUNC;

	S_ppFunction_ONCE_KEY[VK_ESCAPE] = STANDART_ESCAPE;

	return SUCKSEX;
}

void InputSystem::UnInitialize() noexcept {
	if (S_pPressedKeyCodes != nullptr) return;

	delete[] S_pPressedKeyCodes; S_pPressedKeyCodes = nullptr;
	S_pressedKeyCount = 0u;
	delete[] S_pFromKeyCodesToIndex; S_pFromKeyCodesToIndex = nullptr;
	S_isKeysPressed = std::bitset<256>();
	delete[] S_pKeysLParams; S_pKeysLParams = nullptr;


	delete[] S_pPressedMButtons; S_pPressedMButtons = nullptr;
	S_pressedMButtonsCount = 0u;
	delete[] S_pFromMButtonsToIndex; S_pFromMButtonsToIndex = nullptr;
	S_isMouseBPressed = std::bitset<InputSystem::THIS_USED_ONLY_FOR_SETTING_SIZE>();
	delete[] S_pMButtonsInfos; S_pMButtonsInfos = nullptr;

	S_currentCursorPosX = 0;
	S_currentCursorPosY = 0;
	S_cursorDeltaX = 0;
	S_cursorDeltaY = 0;

	S_isCursorInClientArea = false;
	S_wheelDelta = 0;

	delete[] S_ppFunction_ONCE_KEY; S_ppFunction_ONCE_KEY = nullptr;
	delete[] S_ppFunction_KEY_ISPRESSED; S_ppFunction_KEY_ISPRESSED = nullptr;
	delete[] S_ppFunction_KEY_RELEASE; S_ppFunction_KEY_RELEASE = nullptr;
	S_pFunction_ONCHAR = nullptr;

	delete[] S_ppFunction_ONCE_MBUTTON; S_ppFunction_ONCE_MBUTTON = nullptr;
	delete[] S_ppFunction_MOUSE_ISPRESSED; S_ppFunction_MOUSE_ISPRESSED = nullptr;
	delete[] S_ppFunction_MOUSE_RELEASE; S_ppFunction_MOUSE_RELEASE = nullptr;
	S_pFunction_MOVE_MOUSE = nullptr;

	S_pFunction_WHEELUP_STEP = nullptr;
	S_pFunction_WHEELDOWN_STEP = nullptr;

	S_pFunction_OnCursorEnterCA = nullptr;
	S_pFunction_OnCursorLeaveCA = nullptr;
}

void InputSystem::UpdateInput() noexcept {
	for (int i = 0; i < S_pressedKeyCount; i++) {
		InputSystem::S_ppFunction_KEY_ISPRESSED[S_pPressedKeyCodes[i]](S_pKeysLParams[S_pPressedKeyCodes[i]]);
	}

	for (int i = 0; i < S_pressedMButtonsCount; i++) {
		InputSystem::MousePressedInfo info = S_pMButtonsInfos[S_pPressedMButtons[i]];
		InputSystem::S_ppFunction_MOUSE_ISPRESSED[S_pPressedMButtons[i]](info.wParam, info.x, info.y);
	}

	S_cursorDeltaX = 0;
	S_cursorDeltaY = 0;
}

void InputSystem::AddPressedKey(unsigned char keyCode, LPARAM lParam) noexcept {
	if (S_isKeysPressed[keyCode] == 0b1) return;

	S_isKeysPressed[keyCode] = 0b1;
	S_pKeysLParams[keyCode] = lParam;

	S_pPressedKeyCodes[S_pressedKeyCount] = keyCode;
	S_pFromKeyCodesToIndex[keyCode] = S_pressedKeyCount;
	S_pressedKeyCount++;
}

void InputSystem::RemovePressedKey(unsigned char keyCode) noexcept {
	if (S_isKeysPressed[keyCode] == 0b0) return;

	S_isKeysPressed[keyCode] = false;
	S_pKeysLParams[keyCode] = 0;

	unsigned char tmp = S_pFromKeyCodesToIndex[keyCode];
	S_pPressedKeyCodes[tmp] = S_pPressedKeyCodes[S_pressedKeyCount - 1];
	S_pFromKeyCodesToIndex[S_pPressedKeyCodes[tmp]] = tmp;
	S_pressedKeyCount--;
}

void InputSystem::AddPressedMButton(unsigned char mButton, WPARAM wParam, LPARAM lParam) noexcept {
	if (S_isMouseBPressed[mButton] == 0b1) return;

	S_isMouseBPressed[mButton] = 0b1;
	S_pMButtonsInfos[mButton] = { wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)) };

	S_pPressedMButtons[S_pressedMButtonsCount] = mButton;
	S_pFromMButtonsToIndex[mButton] = S_pressedMButtonsCount;
	S_pressedMButtonsCount++;
}

void InputSystem::RemovePressedMButton(unsigned char mButton) noexcept {
	if (S_isMouseBPressed[mButton] == 0b0) return;

	S_isMouseBPressed[mButton] = 0b0;

	unsigned char tmp = S_pFromMButtonsToIndex[mButton];
	S_pPressedMButtons[tmp] = S_pPressedMButtons[S_pressedMButtonsCount - 1];
	S_pFromMButtonsToIndex[S_pPressedMButtons[tmp]] = tmp;
	S_pressedMButtonsCount--;
}

void InputSystem::KillFocus() noexcept {
	for (int i = 0; i < S_pressedKeyCount; i++) {
		S_isKeysPressed[S_pPressedKeyCodes[i]] = false;
	}
	S_pressedKeyCount = 0;

	for (int i = 0; i < S_pressedMButtonsCount; i++) {
		S_isMouseBPressed[S_pPressedMButtons[i]] = false;
	}
	S_pressedMButtonsCount = 0;
}