#include "Collision.h"



Collision::Collision()
{
	float moveSpeed2 = 5.0f;
}


Collision::~Collision()
{

}

int Collision::colliding(float x, float y, float z, float x1, float y1, float z1,float dt)
{
	

	float tempx1 = x;
	tempx1 -= moveSpeed2 * dt;
	if (sqrt(((tempx1 - x1)*(tempx1 - x1)) +
		((y - y1)*(y - y1)) +
		((z - z1)*(z - z1))) < 4.0f)
	{
		return 1;
	}

	tempx1 = x1;
	tempx1 += moveSpeed2 * dt;
	if (sqrt(((x - tempx1)*(x - tempx1)) +
		((y - y1)*(y - y1)) +
		((z - z1)*(z - z1))) < 4.0f)
	{
		return 2;
	}

	float tempz1 = z;
	tempz1 -= moveSpeed2 * dt;
	if (sqrt(((x - x1)*(x - x1)) +
		((y - y1)*(y - y1)) +
		((tempz1 - z1)*(tempz1 - z1))) < 4.0f)
	{
		return 3;
	}

	tempz1 = z;
	tempz1 += moveSpeed2 * dt;
	if (sqrt(((x - x1)*(x - x1)) +
		((y - y1)*(y - y1)) +
		((tempz1 - z1)*(tempz1 - z1))) < 4.0f)
	{
		return 4;
	}
	return 0;
}

