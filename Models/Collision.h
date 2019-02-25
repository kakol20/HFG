#pragma once
#include<math.h>
#include"Dx11DemoBase.h"
#include<xnamath.h>
class Collision
{
public:
	Collision();
	~Collision();

	int colliding(XMFLOAT3 A, XMFLOAT3 B, float dt);

private:
	float moveSpeed2 = 5.0f;
};

