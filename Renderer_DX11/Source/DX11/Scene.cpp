#include "../../Headers/Scene.h"
#include "../../Headers/ShaderSystem.h"
#include "../../Headers/Graphics.h"
#include "../../Headers/Camera.h"
#include "../../Headers/PointLight.h"


Scene::Scene() noexcept {
	D3D11_BUFFER_DESC constBuffDesc{};
	{
		constBuffDesc.ByteWidth = sizeof(m_pixelCBuf0);///must be a multiple of 16
		constBuffDesc.Usage = D3D11_USAGE_DYNAMIC;// D3D11_USAGE_IMMUTABLE can gives more fps
		constBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//
		constBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBuffDesc.MiscFlags = 0u;// miscellaneous
		constBuffDesc.StructureByteStride = 0u;
	}

	D3D11_SUBRESOURCE_DATA constBuffSubResData{};
	{
		constBuffSubResData.pSysMem = &m_pixelCBuf0;
	}

	Graphics::GetDevice()->CreateBuffer(&constBuffDesc, &constBuffSubResData, &m_pPixelCBuf0);
}

Scene::~Scene() noexcept {
	if (m_pPixelCBuf0 != nullptr) {
		m_pPixelCBuf0->Release();
		m_pPixelCBuf0 = nullptr;
	}
}


void Scene::UpdateConstBufs() noexcept {
	if (m_pPixelCBuf0 == nullptr) m_pPixelCBuf0 = Graphics::CreateConstBuffer((char*)&m_pixelCBuf0, sizeof(ShaderSystem::PixelCBuf0));
	Graphics::UpdateConstBuffer(m_pPixelCBuf0, &m_pixelCBuf0, sizeof(ShaderSystem::PixelCBuf0));
	m_shouldUpdateBuf = false;
}


unsigned short Scene::AddPointLight() noexcept {
	if (m_pixelCBuf0.pointLightNum == 64) return -1;

	((PointLight*)m_pixelCBuf0.pointLights)[m_pixelCBuf0.pointLightNum] = PointLight();
	return m_pixelCBuf0.pointLightNum++;
}

void Scene::RemovePointLight(unsigned int index) noexcept {
	if (index >= m_pixelCBuf0.pointLightNum) return;
	--m_pixelCBuf0.pointLightNum;
	if (m_pixelCBuf0.pointLightNum == 0u) return;
	m_pixelCBuf0.pointLights[index] = m_pixelCBuf0.pointLights[m_pixelCBuf0.pointLightNum];
}

PointLight* Scene::GetPointLight(unsigned int index) noexcept {
	m_shouldUpdateBuf = true;
	return (PointLight*) &((PointLight*)m_pixelCBuf0.pointLights)[index];
}

int Scene::GetPointLightsNum() noexcept {
	return m_pixelCBuf0.pointLightNum;
}


void Scene::UpdateViewInf() noexcept {
	m_shouldUpdateBuf = true;
	m_pixelCBuf0.viewPosition = Graphics::GetActiveCamera().GetPosition();
	m_pixelCBuf0.viewDirection = Graphics::GetActiveCamera().GetForward();
	UpdateConstBufs();
}


void Scene::BindLights() noexcept {
	if (m_shouldUpdateBuf) UpdateConstBufs();

	Graphics::GetDeviceContext()->PSSetConstantBuffers(0u, 1u, &m_pPixelCBuf0);
}