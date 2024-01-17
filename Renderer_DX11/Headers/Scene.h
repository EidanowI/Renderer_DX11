#pragma once

#include "../Headers/ShaderSystem.h"
class PointLight;

class Scene
{
public:
	Scene() noexcept;
	~Scene() noexcept;

private:
	void UpdateConstBufs() noexcept;

public:
	unsigned short AddPointLight() noexcept;
	void RemovePointLight(unsigned int index) noexcept;
	PointLight* GetPointLight(unsigned int index) noexcept;
	int GetPointLightsNum() noexcept;

	void UpdateViewInf() noexcept;

	void BindLights() noexcept;

private:

	ShaderSystem::PixelCBuf0 m_pixelCBuf0{};
	ID3D11Buffer* m_pPixelCBuf0 = nullptr;
	bool m_shouldUpdateBuf = true;
};