#pragma once
#include<math.h>
#include"Dx11DemoBase.h"
#include<xnamath.h>
class Collision
{
public:
	Collision();
	~Collision();

	int colliding(XMFLOAT3 A, XMFLOAT3 B, float dt,float dist);

private:
	XMFLOAT3 m_nextP;
	XMVECTOR m_direction;
	float m_speed;
};

