#include "Collision.h"



Collision::Collision()
{
}


Collision::~Collision()
{

}

int Collision::colliding(XMFLOAT3 A, XMFLOAT3 B, float dt,float dist)
{
	m_nextP = A;
	m_direction = XMVector3Normalize(m_direction);
	m_direction = m_direction * m_speed * dt;

	float xDir = XMVectorGetX(m_direction);
	float zDir = XMVectorGetZ(m_direction);

	

	m_nextP.x += xDir;
	m_nextP.z += zDir;
	
	if (sqrt(((m_nextP.x - B.x)*(m_nextP.x - B.x)) +
		((m_nextP.y - B.y)*(m_nextP.y - B.y)) +
		((m_nextP.z - B.z)*(m_nextP.z - B.z))) < dist)
	{
		return 1;
	}
	return 0;
}

