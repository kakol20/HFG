#pragma once
#include<math.h>
class Collision
{
public:
	Collision();
	~Collision();

	int colliding(float x,float y,float z, float x1, float y1, float z1, float dt);

private:
	float moveSpeed2 = 5.0f;
};

