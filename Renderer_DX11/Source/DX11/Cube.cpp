#include "../../Headers/DX11_Base.h"


Mesh<Vertex48B>* Cube::S_pCubeMesh = nullptr;
unsigned int Cube::S_initializedNum = 0;


Cube::Cube() noexcept {
	m_position = GraphicsFundament::Vector3D(GraphicsFundament::RandomMinus11() * 5.0f, GraphicsFundament::RandomMinus11() * 5.0f, GraphicsFundament::RandomMinus11() * 5.0f);
	m_rotation = GraphicsFundament::Vector3D(GraphicsFundament::RandomMinus11() * 3.1415f, GraphicsFundament::RandomMinus11() * 3.1415f, GraphicsFundament::RandomMinus11() * 3.1415f);
	m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));
	m_pVertConstBuf1 = Graphics::CreateConstBuffer<Graphics::VertCBuf1Struct_TestVert>(m_vertConstBuf1Struct);

	UpdateTransform();

	if (S_pCubeMesh != nullptr) return;

	float side = 1.0f / 2.0f;
	unsigned int sourceVerticiesNum = 24u;
	Vertex48B* pSourceVerticies = new Vertex48B[sourceVerticiesNum]{//0 2 1  2 3 1
		{-side,-side,-side, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, RED_A_FLOAT},
		{ side,-side,-side,  0.0f, 0.0f, -1.0f,1.0f, 0.0f, RED_A_FLOAT },
		{ -side,side,-side,	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, RED_A_FLOAT },
		{ side,side,-side, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, RED_A_FLOAT },

		{-side,-side,side,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, BLACK_A_FLOAT },
		{side,-side,side ,  0.0f, 0.0f, 1.0f,1.0f, 0.0f, BLACK_A_FLOAT },
		{-side,side,side ,	 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, BLACK_A_FLOAT },
		{side,side,side ,    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, BLACK_A_FLOAT },

		{-side,-side,-side,  -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, BLACK_A_FLOAT },
		{-side,side,-side,   -1.0f, 0.0f, 0.0f,1.0f, 0.0f, BLACK_A_FLOAT },
		{ -side,-side,side,	 -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, BLACK_A_FLOAT },
		{ -side,side,side,   -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, BLACK_A_FLOAT },

		{side,-side,-side,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, BLACK_A_FLOAT },
		{side,side,-side ,  1.0f, 0.0f, 0.0f,0.0f, 1.0f, BLACK_A_FLOAT },
		{side,-side,side ,	 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, BLACK_A_FLOAT },
		{side,side,side  ,   1.0f, 0.0f, 0.0f, 1.0f, 1.0f, BLACK_A_FLOAT },

		{-side,-side,-side,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, BLACK_A_FLOAT },
		{side,-side,-side ,  0.0f, -1.0f, 0.0f,1.0f, 1.0f, BLACK_A_FLOAT },
		{-side,-side,side ,	 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, BLACK_A_FLOAT },
		{side,-side,side  ,   0.0f, -1.0f, 0.0f, 1.0f, 0.0f, BLACK_A_FLOAT },

		{-side,side,-side,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, BLACK_A_FLOAT },
		{side,side,-side ,  0.0f, 1.0f, 0.0f,1.0f, 1.0f, BLACK_A_FLOAT },
		{-side,side,side ,	 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, BLACK_A_FLOAT },
		{side,side,side  ,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f, BLACK_A_FLOAT },
	};

	unsigned int sourceIndeciesNum = 12u;
	TrianglePoly* pSourceIndecies = new TrianglePoly[sourceIndeciesNum]{
		{0,2,1},{1,2,3},
		{4,5,7},{4,7,6},
		{8,10,9},{10,11,9},
		{12,13,15},{12,15,14},
		{16,17,18},{18,17,19},
		{20,23,21},{20,22,23},
	};

	S_pCubeMesh = new Mesh<Vertex48B>(pSourceVerticies, sourceVerticiesNum, pSourceIndecies, sourceIndeciesNum, "TestVert", "TestPixl");
}
Cube::~Cube() noexcept {
	if (S_pCubeMesh == nullptr) return;
	delete S_pCubeMesh;
	S_pCubeMesh = nullptr;
}

void Cube::UpdateConstBuffer() noexcept {
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));

	Graphics::UpdateConstBuffer<Graphics::VertCBuf1Struct_TestVert>(m_pVertConstBuf1, m_vertConstBuf1Struct);
}

void Cube::Bind() noexcept {
	S_pCubeMesh->Bind();
	Graphics::GetDeviceContext()->VSSetConstantBuffers(1, 1, m_pVertConstBuf1.GetAddressOf());
}
void Cube::Draw() noexcept {
	S_pCubeMesh->Draw();
}