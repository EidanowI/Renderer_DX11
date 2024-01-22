#pragma once

#include "Headers/Graphics.h"
#include "Headers/ShaderSystem.h"
#include "Headers/InputSystem.h"
#include "Headers/Scene.h"
#include "Headers/MeshedObject.h"
#include "Headers/Textures.h"
#include "Headers/Sampler.h"
#include "Headers/Window.h"
#include "Headers/Camera.h"


void Move_Right(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(0.09f, Graphics::GetActiveCamera().m_right);
};
void Move_Left(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(-0.09f, Graphics::GetActiveCamera().m_right);
};
void Move_Forward(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(0.09f, Graphics::GetActiveCamera().m_forward);
};
void Move_Back(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(-0.09f, Graphics::GetActiveCamera().m_forward);
};
void Move_Up(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(0.09f, Graphics::GetActiveCamera().m_up);
};
void Move_Down(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(-0.09f, Graphics::GetActiveCamera().m_up);
};


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
			InputSystem::Initialize();
			InputSystem::S_ppFunction_KEY_ISPRESSED['D'] = Move_Right;
			InputSystem::S_ppFunction_KEY_ISPRESSED['A'] = Move_Left;
			InputSystem::S_ppFunction_KEY_ISPRESSED['W'] = Move_Forward;
			InputSystem::S_ppFunction_KEY_ISPRESSED['S'] = Move_Back;
			InputSystem::S_ppFunction_KEY_ISPRESSED[VK_SPACE] = Move_Up;
			InputSystem::S_ppFunction_KEY_ISPRESSED[VK_SHIFT] = Move_Down;

			Window window = Window(WINDOW_START_WIDTH, WINDOW_START_HEIGHT, WINDOW_START_NAME.c_str());
			Graphics::Initialize(window.GetHinstnce(), &window);
			ERROR_MAINLOOP(MainLoop());
			ShaderSystem::Clear();
			Graphics::UnInitialize();
			InputSystem::UnInitialize();
		} while (SHOULD_CLOSE_WINDOW_AND_CREATE_NEW);

	}

private:
	int MainLoop() noexcept {
		Scene testScene = Scene();

		MeshedObject testMeshed = MeshedObject();
		Graphics::AddCameraAndSetActive(Camera(GraphicsFundament::Vector3D(0.0f, 0.0f, -3.0), GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0)));
		int whiteShader = ShaderSystem::GetPixelShaderIndex("TestWhit", "Shaders/");
		Texture testFredy = Texture(ImageSurface::FromFile("Textures/Fred.png"), 0);
		Sampler testSampler = Sampler();
		testScene.AddPointLight();
		testScene.AddPointLight();

		while (!SHOULD_CLOSE_WINDOW_AND_CREATE_NEW) {
			const std::optional<int> proc = Window::ProcessMsg();
			if (proc.has_value()) return proc.value();

			Graphics::SetRenderTargets();
			Graphics::ClearRenderTarget();

			ImGUIManager::NewFrame();
			ImGUIManager::ShowGUIWindows();

			Graphics::GetActiveCamera().UpdateFpsRotation();
			Graphics::GetActiveCamera().Bind(Camera::VertexShaderName::TestVert);

			testScene.UpdateViewInf();
			testScene.BindLights();

			testSampler.Bind();
			testFredy.Bind();
			testMeshed.Draw();

			
			for (int i = 0; i < testScene.GetPointLightsNum(); i++) {
				ImGui::Begin(("Light " + std::to_string(i)).c_str());
				ImGui::SliderFloat3("Light position: ", &testScene.GetPointLight(i)->m_position.x, -5.0f, 5.0f);
				ImGui::ColorPicker3("Light color: ", & testScene.GetPointLight(i)->m_color.r());
				ImGui::SliderFloat("Specular strength", &testScene.GetPointLight(i)->m_specularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient strength", &testScene.GetPointLight(i)->m_ambientStrength, 0.0f, 1.0f);

				ImGui::InputFloat("Constant", &testScene.GetPointLight(i)->m_constant);
				ImGui::InputFloat("Linear", &testScene.GetPointLight(i)->m_linear);
				ImGui::InputFloat("Quadratic", &testScene.GetPointLight(i)->m_quadratic);
				ImGui::End();
			}

			//ImGui::ShowDemoWindow();
			ImGUIManager::Render();
			Graphics::PresentRenderTargets();

			InputSystem::UpdateInput();
		}

		return SUCKSEX;
	}
};