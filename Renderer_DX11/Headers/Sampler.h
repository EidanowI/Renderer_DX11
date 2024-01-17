#pragma once
#include "Graphics.h"

#include <wrl.h>
#include <d3d11.h>

#include "BindToPipeline.h"
#include "DX11_ErrorMacro.h"

class Sampler : public BindToPipeline {
public:
	Sampler() noexcept;
	void Bind() noexcept override;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampleState;
};