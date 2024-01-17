#pragma once
#include "Graphics.h"

#include <DirectXMath.h>
#include "GparhicsFundamentals.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderSystem.h"
#include "ModelSystem.h"

class MeshedObject : public Transform {
public:
	MeshedObject() noexcept;
	void Draw() noexcept;

private:
	void UpdateConstBuffer() noexcept override;

	ShaderSystem::TestVert_CBuf1 m_vertConstBuf1Struct;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertConstBuf1;

	static Model* S_pModel;

	static constexpr char S_pModelName[] = "FredyFaz";
};
