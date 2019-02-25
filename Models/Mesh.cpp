#include "Mesh.h"

struct VertexPos
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
	XMFLOAT3 norm;
};


Mesh::Mesh()
{
	m_initiated = false;
}

Mesh::Mesh(const Mesh & copyMesh)
{
}

Mesh & Mesh::operator=(const Mesh & copyMesh)
{
	if (this == &copyMesh) return *this;

	m_objModel = copyMesh.m_objModel;
	m_totalVerts = copyMesh.m_totalVerts;

	m_vertexBuffer = 0;
	m_vertexBuffer = copyMesh.m_vertexBuffer;

	// TODO: insert return statement here
	return *this;
}

Mesh::~Mesh()
{
	m_vertexBuffer = 0;
}

bool Mesh::Init(char* name, HRESULT& d3dResult, ID3D11Device* d3dDevice)
{

	if (m_objModel.LoadOBJ(name) == false) {
		DXTRACE_MSG("Error loading 3D model!");
		return false;
	}
	m_totalVerts = m_objModel.GetTotalVerts();

	VertexPos* vertices = new VertexPos[m_totalVerts];
	float* vertsPtr = m_objModel.GetVertices();
	float* texPtr = m_objModel.GetTexCoords();
	float* normalPtr = m_objModel.GetNormals();

	for (int i = 0; i < m_totalVerts; i++)
	{
		vertices[i].pos = XMFLOAT3(*(vertsPtr + 0), *(vertsPtr + 1), *(vertsPtr + 2));
		vertsPtr += 3;

		vertices[i].tex0 = XMFLOAT2(*(texPtr + 0), *(texPtr + 1));
		texPtr += 2;

		vertices[i].norm = XMFLOAT3(*(normalPtr + 0), *(normalPtr + 1), *(normalPtr + 2));
		normalPtr += 3;
	}

	D3D11_BUFFER_DESC vertexDesc;

	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * m_totalVerts;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &m_vertexBuffer);

	/*ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * m_totalVerts;

	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &m_vertexBuffer);*/

	delete[] vertices;
	m_objModel.Release();

	if (FAILED(d3dResult))
	{
		DXTRACE_MSG("Failed to create vertex buffer!");
		return false;
	}

	m_initiated = true;

	return true;
}

bool Mesh::getInitiated()
{
	return m_initiated;
}

int Mesh::getTotalVerts() const
{
	return m_totalVerts;
}

void Mesh::Unload()
{
	if (m_vertexBuffer) m_vertexBuffer->Release();
}

ID3D11Buffer * const * Mesh::getVertexBuffer()
{
	return &m_vertexBuffer;
}
