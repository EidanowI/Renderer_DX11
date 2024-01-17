#pragma once

#include "Graphics.h"
#include <wrl.h>
#include <d3d11.h>

#include "BindToPipeline.h"
#include "ImageSurface.h"
#include "DX11_ErrorMacro.h"

class Texture : public BindToPipeline {
public:
	Texture(const ImageSurface& surface, unsigned int index = 0u) noexcept;
	void Bind() noexcept override;

private:
	unsigned int m_textureBindIndex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSahderResView;
};
