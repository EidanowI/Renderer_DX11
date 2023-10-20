#pragma once
#include "Headers/DX11_Base.h"


constexpr unsigned short WINDOW_START_WIDTH = 1280u;
constexpr unsigned short WINDOW_START_HEIGHT = 720u;
const std::wstring WINDOW_START_NAME = L"DX11 Renderer";
bool SHOULD_CLOSE_WINDOW_AND_CREATE_NEW = false;

class App {
public:
	App() noexcept = default;

	void Run() noexcept {
		do {
			SHOULD_CLOSE_WINDOW_AND_CREATE_NEW = false;

			ImGUIManager imguiManager = ImGUIManager();
			Window window = Window(WINDOW_START_WIDTH, WINDOW_START_HEIGHT, WINDOW_START_NAME.c_str());
			Graphics::Initialize(window.GetHinstnce(), &window);
			ERROR_MAINLOOP(MainLoop());
			Graphics::UnInitialize();
		} while (SHOULD_CLOSE_WINDOW_AND_CREATE_NEW);
	}

private:
	int MainLoop() noexcept {
		while (!SHOULD_CLOSE_WINDOW_AND_CREATE_NEW) {
			const std::optional<int> proc = Window::ProcessMsg();
			if (proc.has_value()) return proc.value();
			Graphics::SetRenderTargets();
			Graphics::ClearRenderTarget();
			ImGUIManager::NewFrame();
			ImGUIManager::ShowGUIWindows();

			//ImGui::ShowDemoWindow();
			ImGUIManager::Render();
			Graphics::PresentRenderTargets();
		}

		return SUCKSEX;
	}
};