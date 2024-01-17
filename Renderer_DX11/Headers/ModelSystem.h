#pragma once

#include "Model.h"

class ModelSystem {
	struct LoadedModel {
		char name[8];
		Model* pModel;
	};
public:
	static Model* GetModel(const char* name, const std::string& extension) noexcept;
	static Model* GetModel(const char* name, const std::string& extension, const std::string& vertexShaderName, const std::string pixelShaderName) noexcept;

private:
	static std::vector<LoadedModel> S_loadedModels;
};
