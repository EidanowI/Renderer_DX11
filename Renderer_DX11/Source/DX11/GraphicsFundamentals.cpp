#include "../../Headers/DX11_Base.h"
#include "../../Headers/DX11_ErrorMacro.h"

float GraphicsFundament::Random01() noexcept{
	return (float(rand() % RAND_MAX)) / (float)RAND_MAX;
}
float GraphicsFundament::RandomMinus11() noexcept {
	return ((float(rand() % RAND_MAX)) / (float)RAND_MAX - 0.5f) * 2.0f;
}

D3D11_INPUT_ELEMENT_DESC Vertex48B::inputElementDesc[4] = {
	{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->position, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->normal, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"UV", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->UV, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"Color", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->color, D3D11_INPUT_PER_VERTEX_DATA,  0u}
};

unsigned char Vertex48B::elementDescNum = 4u;