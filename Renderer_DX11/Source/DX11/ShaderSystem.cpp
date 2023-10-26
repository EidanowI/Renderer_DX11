#include "../../Headers/DX11_Base.h"



std::vector<ShaderSystem::VertexShader> ShaderSystem::S_VertexShaders = std::vector<ShaderSystem::VertexShader>();
std::vector<ShaderSystem::PixelShader>  ShaderSystem::S_PixelShaders = std::vector<ShaderSystem::PixelShader>();


unsigned int ShaderSystem::GetVertexShaderIndex(const char* shader_UID, const std::string& path) noexcept {
	int hres = 0;

	unsigned short VertShadersSIZE = S_VertexShaders.size();
	for (int i = 0; i < VertShadersSIZE; i++) {
		if (S_VertexShaders[i].shader_UID == *(long long*)shader_UID) {
			return i;
		}
	}

	VertexShader vertShader{};
	vertShader.shader_UID = *(long long*)shader_UID;
	std::ifstream ifs;
	ifs.open(path + std::string(shader_UID) + ".cso", std::ios_base::binary | std::ios_base::ate);
	ERROR_FILE(ifs.is_open());
	int ver_SIZE = ifs.tellg();
	ifs.seekg(0);
	vertShader.shaderSourceCode = std::vector<char>(ver_SIZE);
	ifs.read(vertShader.shaderSourceCode.data(), ver_SIZE);
	ifs.close();
	ERROR_HRESULT(Graphics::GetDevice()->CreateVertexShader(vertShader.shaderSourceCode.data(), ver_SIZE, nullptr, &vertShader.vertexShader));
	S_VertexShaders.push_back(vertShader);
	return VertShadersSIZE;
}

unsigned int ShaderSystem::GetPixelShaderIndex(const char* shader_UID, const std::string& path) noexcept {
	int hres = 0;

	unsigned short PixelShadersSIZE = S_PixelShaders.size();
	for (int i = 0; i < PixelShadersSIZE; i++) {
		if (S_PixelShaders[i].shader_UID == *(long long*)shader_UID) {
			return i;
		}
	}

	PixelShader pixelShader{};
	pixelShader.shader_UID = *(long long*)shader_UID;
	std::ifstream ifs;
	ifs.open(path + std::string(shader_UID) + ".cso", std::ios_base::binary | std::ios_base::ate);
	ERROR_FILE(ifs.is_open());
	int pix_SIZE = ifs.tellg();
	ifs.seekg(0);
	pixelShader.shaderSourceCode = std::vector<char>(pix_SIZE);
	ifs.read(pixelShader.shaderSourceCode.data(), pix_SIZE);
	ifs.close();
	ERROR_HRESULT(Graphics::GetDevice()->CreatePixelShader(pixelShader.shaderSourceCode.data(), pix_SIZE, nullptr, &pixelShader.pixelShader));
	S_PixelShaders.push_back(pixelShader);
	return PixelShadersSIZE;
}