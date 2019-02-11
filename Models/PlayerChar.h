#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H

#include"Dx11DemoBase.h"
#include "Mesh.h"
#include "Texture.h"

class Player
{
public:
	Player();
	~Player();

	void setMesh(Mesh * mesh);
	void setTexture(Texture* texture);

	Mesh* getMesh();
	Texture* getTexture();

	XMMATRIX getWorldMat();

	void setScale(XMFLOAT3 scale);
	void setDirection(XMVECTOR direction);
	void setPosition(XMFLOAT3 position);

	void moveForward(float dt);
	void moveRight();
	void update(XMFLOAT3 opponentPostion);
	void updateWorldMat();

	XMFLOAT3 getPosition();
	XMVECTOR getDirection();

private:
	Mesh * m_mesh;
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
};

#endif // !PLAYERCHAR_H