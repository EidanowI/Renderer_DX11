#include <d3d11.h>

#include "../../Headers/Model.h"
#include "../../Headers/DX11_ErrorMacro.h"
#include "../../Headers/ShaderSystem.h"
#include "../../Headers/Mesh.h"
#include "../../Headers/Graphics.h"


Model::Model(const std::string& path, short&& vertexShaderIndex, short&& pixelShaderIndex) noexcept : m_vertexShaderIndex(vertexShaderIndex), m_pixelShaderIndex(pixelShaderIndex) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate);
	if (scene == nullptr ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
#ifdef  IS_DEBUG
		///TODO Loging
		__ERROR(L"Unable to load model", __FILE__);		
		std::ofstream ofs;
		ofs.open("Logs/Log.txt");
		ofs.write(importer.GetErrorString(), 256);
		ofs.close();
#endif //
		return;
	}
	m_meshes = std::vector<char*>();
	LoadNode(scene->mRootNode, scene);
}
Model::Model(const std::string& path) noexcept : Model(path, ShaderSystem::GetVertexShaderIndex("TestVert", "Shaders/"), ShaderSystem::GetPixelShaderIndex("TestPixl", "Shaders/")) {}
Model::~Model() noexcept {
	for (int i = 0; i < m_meshes.size(); i++) {
		delete ((Mesh*)m_meshes[i]);
	}
}

void Model::LoadNode(const aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back((char*)LoadMesh(mesh, scene));
	}

	for (unsigned int i = 0; i< node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}
Mesh* Model::LoadMesh(const aiMesh* mesh, const aiScene* scene) {
	unsigned int verticiesNums = mesh->mNumVertices;
	Mesh::Vertex48B* verticies = new Mesh::Vertex48B[verticiesNums];
	unsigned int indeciesNum = mesh->mNumFaces;
	Mesh::TrianglePoly* indecies = new Mesh::TrianglePoly[indeciesNum];

	bool isMeshContainsUV = mesh->mTextureCoords[0];
	int theBigOne = 0;
	
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		verticies[i].position = GraphicsFundament::Vector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		verticies[i].normal = GraphicsFundament::Vector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (isMeshContainsUV)
		{
			verticies[i].UV = GraphicsFundament::Vector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else 
		{
			verticies[i].UV = GraphicsFundament::Vector2D(0.0f, 0.0f);
		}
		if (mesh->mColors[3]) {
			verticies[i].color = GraphicsFundament::Vector4D(mesh->mColors[i]->r, mesh->mColors[i]->g, mesh->mColors[i]->b, mesh->mColors[i]->a);
		}
		else {
			verticies[i].color = GraphicsFundament::Vector4D(0.0f,0.0f,0.0f,0.0f);
		}
	}
	for (int i = 0; i < indeciesNum; i++) {
		indecies[i].I1 = (unsigned short)mesh->mFaces[i].mIndices[0];
		indecies[i].I2 = (unsigned short)mesh->mFaces[i].mIndices[1];
		indecies[i].I3 = (unsigned short)mesh->mFaces[i].mIndices[2];
		theBigOne = max(theBigOne, indecies[i].I1);
		theBigOne = max(theBigOne, indecies[i].I2);
		theBigOne = max(theBigOne, indecies[i].I3);
	}

	return new Mesh(verticies, verticiesNums,
		indecies,indeciesNum,
		m_vertexShaderIndex);
}


void Model::Draw(ID3D11Buffer** ppVConstBufs, unsigned char startSlot, unsigned char numBuffs) noexcept {
	Graphics::GetDeviceContext()->VSSetConstantBuffers(startSlot, numBuffs, ppVConstBufs);

	Graphics::GetDeviceContext()->VSSetShader(ShaderSystem::S_VertexShaders[m_vertexShaderIndex].vertexShader.Get(), nullptr, 0u);
	Graphics::GetDeviceContext()->PSSetShader(ShaderSystem::S_PixelShaders[m_pixelShaderIndex].pixelShader.Get(), nullptr, 0u);

	for (int i = 0; i < m_meshes.size(); i++) {
		((Mesh*)m_meshes[i])->Bind();
		((Mesh*)m_meshes[i])->Draw();
	}
}
void Model::Draw() noexcept {
	Graphics::GetDeviceContext()->VSSetShader(ShaderSystem::S_VertexShaders[m_vertexShaderIndex].vertexShader.Get(), nullptr, 0u);
	Graphics::GetDeviceContext()->PSSetShader(ShaderSystem::S_PixelShaders[m_pixelShaderIndex].pixelShader.Get(), nullptr, 0u);

	for (int i = 0; i < m_meshes.size(); i++) {
		((Mesh*)m_meshes[i])->Bind();
		((Mesh*)m_meshes[i])->Draw();
	}
}