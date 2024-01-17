#pragma once

#include <Windows.h>

#include "Windows.h"
#include "ImGUIManager.h"
#include "DX11_ErrorMacro.h"

#include <bitset>
#include <vector>

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
