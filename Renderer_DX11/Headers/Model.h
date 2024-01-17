#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include <fstream>

#include "../Dependencies/assimp/include/assimp/Importer.hpp";
#include "../Dependencies/assimp/include/assimp/scene.h";
#include "../Dependencies/assimp/include/assimp/postprocess.h";


class Mesh;

class Model {
public:
	Model(const std::string& path, short&& vertexShaderIndex, short&& pixelShaderIndex) noexcept;
	Model(const std::string& path) noexcept;
	~Model() noexcept;

	void Draw(ID3D11Buffer** ppVConstBufs, unsigned char startSlot, unsigned char numBuffs) noexcept;
	void Draw() noexcept;

private:
	void LoadNode(const aiNode* node, const aiScene* scene);
	Mesh* LoadMesh(const aiMesh* mesh, const aiScene* scene);

	std::vector<char*> m_meshes;

	unsigned short m_vertexShaderIndex;
	unsigned short m_pixelShaderIndex;
};