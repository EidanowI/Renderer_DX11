#pragma once
#include "Headers/DX11_Base.h"

float cameraXRotation = -0.374;
float cameraYRotation = 2.726;
float cameraZRotation = 0;

float cameraPosX = 2.69;
float cameraPosY = 2.92;
float cameraPosZ = -5.59;

float POWFACTOR = 140;

#define POINT_LIGHT_NUM 4
struct PointLight {
	float x;
	float y;
	float z;
	float specularStrength;

	float r;
	float g;
	float b;
	float ambientStrength;

	float constant;
	float linear;
	float quadratic;
	float smth;
};


PointLight testLights[POINT_LIGHT_NUM] =
{ 
	{0,0,0,0.5f,  1,1,1,0.15f,   1.0f, 0.7f, 1.8f, 0.0f},
	{0,0,0,0.5f,  1,1,1,0.15f,   1.0f, 0.7f, 1.8f, 0.0f} ,
	{0,0,0,0.5f,  1,1,1,0.15f,   1.0f, 0.7f, 1.8f, 0.0f} ,
	{0,0,0,0.5f,  1,1,1,0.15f,   1.0f, 0.7f, 1.8f, 0.0f} 
};


void GuiMatrix(const DirectX::XMMATRIX& matrix, const std::string& name) noexcept{
	//std::string name = "ShowMatrix" + std::to_string(rand() * rand());
	ImGui::Begin(name.c_str());
	ImGui::BulletText(std::to_string((*((float*)&matrix + 0))).c_str());
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 4))).c_str());//4
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 8))).c_str());//8
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 12))).c_str());//12

	ImGui::BulletText(std::to_string((*((float*)&matrix + 1))).c_str());//1
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 5))).c_str());//5
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 9))).c_str());//9
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 13))).c_str());//13

	ImGui::BulletText(std::to_string((*((float*)&matrix + 2))).c_str());//2
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 6))).c_str());//6
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 10))).c_str());//10
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 14))).c_str());//14

	ImGui::BulletText(std::to_string((*((float*)&matrix + 3))).c_str());//3
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 7))).c_str());//7
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 11))).c_str());//11
	ImGui::SameLine();
	ImGui::BulletText(std::to_string((*((float*)&matrix + 15))).c_str());//15
	ImGui::End();
}
void Move_Right(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(0.04f, Graphics::GetActiveCamera().m_right);
};
void Move_Left(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(-0.04f, Graphics::GetActiveCamera().m_right);
};
void Move_Forward(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(0.04f, Graphics::GetActiveCamera().m_forward);
};
void Move_Back(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(-0.04f, Graphics::GetActiveCamera().m_forward);
};
void Move_Up(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(0.04f, Graphics::GetActiveCamera().m_up);
};
void Move_Down(LPARAM lParam) noexcept {
	Graphics::GetActiveCamera().TranslatePosition(-0.04f, Graphics::GetActiveCamera().m_up);
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
		Cube testCube = Cube();
		Cube lightCube[4] = { Cube(), Cube(), Cube(), Cube()};
		lightCube[0].SetScale(0.1f, 0.1f, 0.1f);
		lightCube[1].SetScale(0.1f, 0.1f, 0.1f);
		lightCube[2].SetScale(0.1f, 0.1f, 0.1f);
		lightCube[3].SetScale(0.1f, 0.1f, 0.1f);
		testCube.SetPosition(0.0f, 0.0f, 0.0f);

		Graphics::AddCameraAndSetActive(Camera(GraphicsFundament::Vector3D(0.0f, 0.0f, -3.0), GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0)));

		int whiteShader = ShaderSystem::GetPixelShaderIndex("TestWhit", "Shaders/");

		Texture testTexture = Texture(ImageSurface::FromFile("Textures/Brick_Color.png"), 0);
		Texture flashlght = Texture(ImageSurface::FromFile("Textures/FlashLight.png"), 2);
		Sampler testSampler = Sampler();

		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertCBUF0;
		struct VertBuf0 {
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};
		VertBuf0 V0Buff{};
		V0Buff.view = Graphics::GetActiveCamera().m_view;
		V0Buff.projection = Graphics::GetActiveCamera().m_projection;
		pVertCBUF0 = Graphics::CreateConstBuffer<VertBuf0>(V0Buff);
		Graphics::GetDeviceContext()->VSSetConstantBuffers(0, 1, pVertCBUF0.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Buffer> pPixelCBUF0;
		struct PixelBuf0 {
			struct {
				float x;
				float y;
				float z;
				float powFactor;
			} viewAndPowFactor;
			struct {
				float x;
				float y;
				float z;
				float cutOffCosine;
			} viewDirection;
			PointLight pointLights[POINT_LIGHT_NUM];
		};
		PixelBuf0 P0Buff{};
		GraphicsFundament::Vector3D tmp = Graphics::GetActiveCamera().GetPosition();
		P0Buff.viewAndPowFactor = { tmp.x, tmp.y, tmp.z, POWFACTOR};
		tmp = Graphics::GetActiveCamera().GetForward();
		P0Buff.viewDirection = { tmp.x, tmp.y, tmp.z, 0.0f };
		for (int i = 0; i < POINT_LIGHT_NUM; i++) {
			P0Buff.pointLights[i] = testLights[i];
		}
		pPixelCBUF0 = Graphics::CreateConstBuffer<PixelBuf0>(P0Buff);
		Graphics::GetDeviceContext()->PSSetConstantBuffers(0, 1, pPixelCBUF0.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11Buffer> pLightPixelCBUF0;
		struct LightShaderPixelBuffer {
			struct {
				float r;
				float g;
				float b;
				float a;
			} color;
		};
		LightShaderPixelBuffer PLight0Buff{0.0f, 0.0f, 0.0f, 0.0f};
		
		pLightPixelCBUF0 = Graphics::CreateConstBuffer<LightShaderPixelBuffer>(PLight0Buff);
		Graphics::GetDeviceContext()->PSSetConstantBuffers(0, 1, pPixelCBUF0.GetAddressOf());



		while (!SHOULD_CLOSE_WINDOW_AND_CREATE_NEW) {
			const std::optional<int> proc = Window::ProcessMsg();
			if (proc.has_value()) return proc.value();

			Graphics::SetRenderTargets();
			Graphics::ClearRenderTarget();

			ImGUIManager::NewFrame();
			ImGUIManager::ShowGUIWindows();

			Graphics::GetActiveCamera().SetCameraFPSRotation(cameraXRotation, cameraYRotation, cameraZRotation);

			V0Buff.view = Graphics::GetActiveCamera().m_view; 
			V0Buff.projection = Graphics::GetActiveCamera().m_projection;
			Graphics::UpdateConstBuffer<VertBuf0>(pVertCBUF0, V0Buff);
			Graphics::GetDeviceContext()->VSSetConstantBuffers(0, 1, pVertCBUF0.GetAddressOf());


			tmp = Graphics::GetActiveCamera().GetPosition();
			P0Buff.viewAndPowFactor = {tmp .x, tmp.y, tmp.z, POWFACTOR};
			tmp = Graphics::GetActiveCamera().GetForward();
			P0Buff.viewDirection = { tmp.x, tmp.y, tmp.z, 0.0f };
			for (int i = 0; i < POINT_LIGHT_NUM; i++) {
				P0Buff.pointLights[i] = testLights[i];
			}
			Graphics::UpdateConstBuffer<PixelBuf0>(pPixelCBUF0, P0Buff);
			Graphics::GetDeviceContext()->PSSetConstantBuffers(0, 1, pPixelCBUF0.GetAddressOf());

			testTexture.Bind();
			testSampler.Bind();
			testCube.Bind();
			testCube.Draw();

			for (int i = 0; i < POINT_LIGHT_NUM; i++) {
				lightCube[i].SetPosition(testLights[i].x, testLights[i].y, testLights[i].z);
				lightCube[i].Bind();
				Graphics::GetDeviceContext()->PSSetShader(ShaderSystem::S_PixelShaders[whiteShader].pixelShader.Get(), nullptr, NULL);
				PLight0Buff.color = { testLights[i].r, testLights[i].g, testLights[i].b, 1.0f };
				Graphics::UpdateConstBuffer<LightShaderPixelBuffer>(pLightPixelCBUF0, PLight0Buff);
				Graphics::GetDeviceContext()->PSSetConstantBuffers(0u, 1u, pLightPixelCBUF0.GetAddressOf());
				lightCube[i].Draw();
			}
			
			for (int i = 0; i < POINT_LIGHT_NUM; i++) {
				ImGui::Begin(("Light " + std::to_string(i)).c_str());
				ImGui::SliderFloat3("Light position: ", &testLights[i].x, -5.0f, 5.0f);
				ImGui::ColorPicker3("Light color: ", &testLights[i].r);
				ImGui::SliderFloat("Specular strength", &testLights[i].specularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient strength", &testLights[i].ambientStrength, 0.0f, 1.0f);

				ImGui::InputFloat("Constant", &testLights[i].constant);
				ImGui::InputFloat("Linear", &testLights[i].linear);
				ImGui::InputFloat("Quadratic", &testLights[i].quadratic);
				ImGui::End();
			}

			ImGui::Begin("Camera");
			ImGui::SliderFloat("Camear rotation X: ", &cameraXRotation, -3.1415f, 3.1415f);
			ImGui::SliderFloat("Camear rotation Y: ", &cameraYRotation, -3.1415f, 3.1415f);
			ImGui::SliderFloat("Camear rotation Z: ", &cameraZRotation, -3.1415f, 3.1415f);
			ImGui::SliderFloat("Camear position X: ", &cameraPosX, -10, 10);
			ImGui::SliderFloat("Camear position Y: ", &cameraPosY, -10, 10);
			ImGui::SliderFloat("Camear position Z: ", &cameraPosZ, -10, 10);
			ImGui::End();

			//ImGui::ShowDemoWindow();
			ImGUIManager::Render();
			Graphics::PresentRenderTargets();

			InputSystem::UpdateInput();
		}

		return SUCKSEX;
	}
};