#include "../../Headers/MeshedObject.h"

Model* MeshedObject::S_pModel = nullptr;

MeshedObject::MeshedObject() noexcept {
	m_position = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_rotation = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));
	m_pVertConstBuf1 = Graphics::CreateConstBuffer((char*) & m_vertConstBuf1Struct, sizeof(ShaderSystem::TestVert_CBuf1));

	UpdateTransform();

	if (S_pModel == nullptr) S_pModel = ModelSystem::GetModel(S_pModelName, ".dae", "TestVert", "TestPixl");
}

void MeshedObject::Draw() noexcept {
	S_pModel->Draw(m_pVertConstBuf1.GetAddressOf(), 1u, 1u);
}

void MeshedObject::UpdateConstBuffer() noexcept{
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));

	Graphics::UpdateConstBuffer(m_pVertConstBuf1, &m_vertConstBuf1Struct, sizeof(ShaderSystem::TestVert_CBuf1));
}