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
	//void setDirection(XMVECTOR direction);
	void setPosition(XMFLOAT3 position);

	void moveForward(float dt, bool reverse);
	void moveRight(float dt, bool reverse);
	void update(XMFLOAT3 opponentPosition);
	void updateWorldMat();
	void SetPlayer(bool player);
	XMFLOAT3 getPosition();
	XMVECTOR getDirection();

private:
	Mesh * m_mesh;
	Texture* m_texture;
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;

	XMMATRIX m_worldMat;
	XMMATRIX m_translation;
	XMMATRIX m_rotation;
	XMMATRIX m_scaleMat;

	XMVECTOR m_defaultForward;
	XMVECTOR m_defaultRight;
	XMVECTOR m_defaultUp;

	XMVECTOR m_direction;

	float m_health;
	float m_speed;
	float m_strength;

	bool m_player1 = true;
};

#endif // !PLAYERCHAR_H
