#ifndef MESH_H
#define MESH_H

#include"Dx11DemoBase.h"
//#include "Texture.h"
#include "objLoader.h"
#include<xnamath.h>


class Mesh {

public:
	Mesh();
	Mesh(const Mesh& copyMesh);
	Mesh& operator=(const Mesh& copyMesh);
	~Mesh();

	bool Init(const char * name, HRESULT&  d3dResult, ID3D11Device* d3dDevice);

	bool getInitiated();

	int getTotalVerts() const;

	void Unload();

	ID3D11Buffer* const* getVertexBuffer();

private:
	//char m_name[60];

	ObjModel m_objModel;
	int m_totalVerts;
	bool m_initiated;

	ID3D11Buffer* m_vertexBuffer;
};
#endif // !MESH_H
