#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include"Dx11DemoBase.h"
#include "Mesh.h"
#include "Texture.h"

#include <math.h>
#include <string>

class GameObject {
public:
	GameObject();
	GameObject(const GameObject &copyObject);
	GameObject& operator=(const GameObject &copyObject);
	~GameObject();

	void setMesh(Mesh* mesh);
	void setTexture(Texture* texture);

	Mesh* getMesh();
	Texture* getTexture();

	XMMATRIX getWorldMat();

	void setDirection(XMVECTOR direction);
	void setScale(XMFLOAT3 scale);

	void moveForward(float speed);
	
	void setPosition(XMFLOAT3 position);
	XMFLOAT3 getPosition();
	XMVECTOR getDirection();

	void updateWorldMat();

	std::string GetID() { return m_id; };
	void SetID(const std::string & id) { m_id = id; };

	//void draw(ID3D11DeviceContext* d3dContext, HRESULT &d3dResult, ID3D11Device *d3dDevice, const UINT *pStrides, const UINT *pOffset);

private:
	Mesh* m_mesh;
	Texture* m_texture;
	XMFLOAT3 m_position;

	XMMATRIX m_worldMat;
	XMMATRIX m_translation;
	XMMATRIX m_rotation;
	XMMATRIX m_scale;

	XMVECTOR m_defaultForward;
	XMVECTOR m_defaultRight;
	XMVECTOR m_defaultUp;

	XMVECTOR m_direction;

	std::string m_id;
};
#endif // !GAMEOBJECT_H
