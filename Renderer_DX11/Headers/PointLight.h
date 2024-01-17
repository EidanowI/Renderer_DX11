#pragma once

#include "GparhicsFundamentals.h"
#include "Model.h"
#include "ModelSystem.h"

class PointLight {
public:
	PointLight() noexcept;
	PointLight(GraphicsFundament::Vector3D position) noexcept;

	GraphicsFundament::Vector3D m_position;
	float m_specularStrength;
	GraphicsFundament::Vector4D m_color;
	float m_constant;
	float m_linear;
	float m_quadratic;
	float m_ambientStrength;
};