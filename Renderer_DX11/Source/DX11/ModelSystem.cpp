#include "../../Headers/ModelSystem.h"
#include "../../Headers/ShaderSystem.h"

std::vector<ModelSystem::LoadedModel> ModelSystem::S_loadedModels = std::vector< ModelSystem::LoadedModel>();
Model* ModelSystem::GetModel(const char* name, const std::string& extension, const std::string& vertexShaderName, const std::string pixelShaderName) noexcept {
	for (int i = 0; i < S_loadedModels.size(); i++) {
		bool modelExist = true;
		for (int j = 0; j < 8; j++) {
			if (S_loadedModels[i].name[j] != name[j]) 
			{
				modelExist = false;
				break;
			}
		}
		if (modelExist) {
			return S_loadedModels[i].pModel;
		}

	}

	LoadedModel loadedModel{};
	memcpy(loadedModel.name, name, 8);
	loadedModel.pModel = new Model("Models/" + std::string(name) + extension, ShaderSystem::GetVertexShaderIndex(vertexShaderName.c_str(), "Shaders/"), ShaderSystem::GetPixelShaderIndex(pixelShaderName.c_str(), "Shaders/"));
	S_loadedModels.push_back(loadedModel);
	return loadedModel.pModel;
}
Model* ModelSystem::GetModel(const char* name, const std::string& extension) noexcept {
	return GetModel(name, extension, "TestVert", "TestPixl");
}