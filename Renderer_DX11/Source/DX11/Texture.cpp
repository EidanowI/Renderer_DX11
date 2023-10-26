#include "../../Headers/DX11_Base.h"



Texture::Texture(const ImageSurface& surface, unsigned int index) noexcept : m_textureBindIndex(index) {
	int hres = 0;
	D3D11_TEXTURE2D_DESC textureDesc{};
	{
		textureDesc.Width = surface.GetWidth();
		textureDesc.Height = surface.GetHeight();
		textureDesc.MipLevels = 1u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0u;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = 0u;
	}
	D3D11_SUBRESOURCE_DATA subresData{};
	{
		subresData.pSysMem = surface.GetBuffer();
		subresData.SysMemPitch = surface.GetWidth() * sizeof(ImageSurface::Color);
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	ERROR_HRESULT(Graphics::GetDevice()->CreateTexture2D(&textureDesc, &subresData, &pTexture));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDesc = {};
	{
		shaderResViewDesc.Format = textureDesc.Format;
		shaderResViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResViewDesc.Texture2D.MipLevels = 1;
	}
	ERROR_HRESULT(Graphics::GetDevice()->CreateShaderResourceView(
		pTexture.Get(), &shaderResViewDesc, &m_pSahderResView
	));
}

void Texture::Bind() noexcept {
	Graphics::GetDeviceContext()->PSSetShaderResources(m_textureBindIndex, 1u, m_pSahderResView.GetAddressOf());
}