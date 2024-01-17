#pragma once

#include "Graphics.h"
#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_impl_win32.h"
#include "../Dependencies/imGUI/imgui_impl_dx11.h"

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
