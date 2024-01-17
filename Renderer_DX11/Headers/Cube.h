#pragma once
#include "Graphics.h"

#include <wrl.h>
#include <d3d11.h>

#include "Mesh.h"
#include "Transform.h"
#include "ShaderSystem.h"
#include "GparhicsFundamentals.h"

class Cube : public Transform {
public:
	Cube() noexcept;
	~Cube() noexcept;

	void UpdateConstBuffer() noexcept override;

	void Bind() noexcept;
	void Draw() noexcept;

private:
	ShaderSystem::TestVert_CBuf1 m_vertConstBuf1Struct;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertConstBuf1;

	static Mesh* S_pCubeMesh;
};