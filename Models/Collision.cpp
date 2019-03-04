#include "Collision.h"



Collision::Collision()
{
	float moveSpeed2 = 5.0f;
}


Collision::~Collision()
{

}

int Collision::colliding(XMFLOAT3 A, XMFLOAT3 B, float dt)
{
	

	float tempx1 = A.x;
	tempx1 -= moveSpeed2 * dt;
	if (sqrt(((tempx1 - B.x)*(tempx1 - B.x)) +
		((A.y - B.y)*(A.y - B.y)) +
		((A.z - B.z)*(A.z - B.z))) < 4.0f)
	{
		return 1;
	}

	tempx1 = A.x;
	tempx1 += moveSpeed2 * dt;
	if (sqrt(((tempx1 - B.x)*(tempx1 - B.x)) +
		((A.y - B.y)*(A.y - B.y)) +
		((A.z - B.z)*(A.z - B.z))) < 4.0f)
	{
		return 2;
	}

	float tempz1 = A.z;
	tempz1 -= moveSpeed2 * dt;
	if (sqrt(((A.x - B.x)*(A.x - B.x)) +
		((A.y - B.y)*(A.y - B.y)) +
		((tempz1 - B.z)*(tempz1 - B.z))) < 4.0f)
	{
		return 3;
	}

	tempz1 = A.z;
	tempz1 += moveSpeed2 * dt;
	if (sqrt(((A.x - B.x)*(A.x - B.x)) +
		((A.y - B.y)*(A.y - B.y)) +
		((tempz1 - B.z)*(tempz1 - B.z))) < 4.0f)
	{
		return 4;
	}
	return 0;
}

