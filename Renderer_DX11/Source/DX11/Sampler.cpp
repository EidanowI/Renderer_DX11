#include "../../Headers/Sampler.h"



Sampler::Sampler() noexcept {
	int hres = 0;
	D3D11_SAMPLER_DESC samplerDesc{};
	{
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	}
	ERROR_HRESULT(Graphics::GetDevice()->CreateSamplerState(&samplerDesc, &m_pSampleState));
}

void Sampler::Bind() noexcept {
	Graphics::GetDeviceContext()->PSSetSamplers(0u, 1u, m_pSampleState.GetAddressOf());
}