#include "../../Headers/DX11_Base.h"

Camera::Camera(const GraphicsFundament::Vector3D& pos, const GraphicsFundament::Vector3D& rotation) noexcept {
	m_position = pos;
	m_camOrientation = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, (float)Window::GetWindowWidth() / (float)Window::GetWindowHeight(), 0.1f, 100.0f);
	UpdateVectors();
}

void Camera::UpdateVectors() noexcept {
	DirectX::XMVECTOR tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), m_camOrientation);
	m_right = GraphicsFundament::Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));

	tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), m_camOrientation);
	m_up = GraphicsFundament::Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));

	tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), m_camOrientation);
	m_forward = GraphicsFundament::Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));
}
void Camera::SetCameraFPSRotation(float x, float y, float z) noexcept {
	m_camOrientation = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), z);
	m_camOrientation *= DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), x);
	m_camOrientation *= DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), y);
	UpdateVectors();
	GraphicsFundament::Vector3D lookDir = m_position + m_forward;
	DirectX::XMVECTOR lookTarget = DirectX::XMVectorSet(
		lookDir.x, lookDir.y, lookDir.z,
		0.0f)
		;
	m_view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.0f), lookTarget, DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 1.0f));
}
void Camera::TranslatePosition(float a, GraphicsFundament::Vector3D& direction) noexcept {
	m_position += (direction * a);
}