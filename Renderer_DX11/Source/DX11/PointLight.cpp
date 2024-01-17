#include "../../Headers/PointLight.h"

PointLight::PointLight(GraphicsFundament::Vector3D position) noexcept {
	m_position = position;
	m_specularStrength = 0.5f;
	m_color = GraphicsFundament::Vector4D(1.0f);
	m_constant = 1.0f;
	m_linear = 0.09f;
	m_quadratic = 0.032f;
	m_ambientStrength = 0.0f;
}
PointLight::PointLight() noexcept : PointLight(GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f)) {};
