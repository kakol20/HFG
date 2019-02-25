#include "PlayerChar.h"

Player::Player()
{
	m_defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_direction = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_position = { 0.0f, 0.0f, 0.0f };

	m_scale = { 1.0f, 1.0f, 1.0f };
	m_scaleMat = XMMatrixIdentity();
	m_scaleMat = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	m_worldMat = XMMatrixIdentity();
	m_translation = XMMatrixIdentity();
	m_rotation = XMMatrixIdentity();

	m_animation = "n/a";

	m_speed = 2.0f;
}

Player::~Player()
{
}

void Player::setMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

void Player::setTexture(Texture * texture)
{
	m_texture = texture;
}

Mesh * Player::getMesh()
{
	if (m_animation == "walk")
	{
		return m_walkAnim[m_currFrame];
	}
	
	return m_mesh;
}

Texture * Player::getTexture()
{
	return m_texture;
}

XMMATRIX Player::getWorldMat()
{
	return m_worldMat;
}

void Player::setScale(XMFLOAT3 scale)
{
	m_scale = scale;

	updateWorldMat();
}

void Player::setPosition(XMFLOAT3 position)
{
	//m_translation = XMMatrixIdentity();

	m_position = position;

	//m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	updateWorldMat();
}

void Player::moveForward(float dt, bool reverse)
{
	m_direction = XMVector3Normalize(m_direction);
	m_direction = m_direction * m_speed * dt;

	float xDir = XMVectorGetX(m_direction);
	float zDir = XMVectorGetZ(m_direction);

	if (reverse) // boolean for checking to move in the reverse direction
	{
		xDir *= -1;
		zDir *= -1;
	}

	m_position.x += xDir;
	m_position.z += zDir;

	/*updateWorldMat();*/
}

void Player::moveRight(float dt, bool reverse)
{
	m_direction = XMVector3Normalize(m_direction);
	m_direction = m_direction * m_speed * dt;

	float xDir = -1.0f * XMVectorGetZ(m_direction);
	float zDir = XMVectorGetX(m_direction);

	if (reverse) // boolean for checking to move in the reverse direction
	{
		xDir *= -1;
		zDir *= -1;
	}

	m_position.x += xDir;
	m_position.z += zDir;

	/*updateWorldMat();*/
}

void Player::update(float dt, XMFLOAT3 opponentPosition)
{
	// animation

	if (m_animation == "walk")
	{
		m_dtCumulative += dt;
		if (m_dtCumulative >= (1 / 12.0f))
		{
			m_dtCumulative = 0.0f;
			m_currFrame++;

			if (m_currFrame == 7)
			{
				m_currFrame = 0;
			}
		}
	}

	m_direction = XMVectorSet(opponentPosition.x - m_position.x, opponentPosition.y - m_position.y, opponentPosition.z - m_position.z, 0.0f);
	//m_direction = XMdVector3ClampLength(m_direction, 0.0f, 1.0f);
	m_direction = XMVector3Normalize(m_direction);

	updateWorldMat();
}

void Player::updateWorldMat()
{
	//m_scale = XMMatrixIdentity();
	m_translation = XMMatrixIdentity();
	m_worldMat = XMMatrixIdentity();
	m_rotation = XMMatrixIdentity();
	m_scaleMat = XMMatrixIdentity();

	m_scaleMat = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	XMVECTOR invertX = XMVectorSet(-1.0f*XMVectorGetX(m_direction), XMVectorGetY(m_direction), XMVectorGetZ(m_direction), 0.0f);

	XMVECTOR xAxis = XMVector3Cross(m_defaultUp, invertX);
	xAxis = XMVector3Normalize(xAxis);

	m_rotation = XMMatrixSet(
		XMVectorGetX(xAxis), XMVectorGetX(m_defaultUp), XMVectorGetX(invertX), 0.0f,
		XMVectorGetY(xAxis), XMVectorGetY(m_defaultUp), XMVectorGetY(invertX), 0.0f,
		XMVectorGetZ(xAxis), XMVectorGetZ(m_defaultUp), XMVectorGetZ(invertX), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	m_worldMat = m_scaleMat * m_rotation * m_translation;

	//m_worldMat =  m_translation;

	m_worldMat = XMMatrixTranspose(m_worldMat);
}

void Player::setIsAnimated(bool animated)
{
	m_isAnimated = true;
}

bool Player::getIsAnimated()
{
	return m_isAnimated;
}

void Player::setAnimation(const std::string & name)
{
	m_animation = name;
}

void Player::setIdleMesh(Mesh * meshFrames[])
{
	for (int i = 0; i < 8; i++)
	{
		m_idleAnim[i] = meshFrames[i];
	}
}

void Player::setWalkMesh(Mesh * meshFrames[])
{
	for (int i = 0; i < 8; i++)
	{
		m_walkAnim[i] = meshFrames[i];
	}
}


XMFLOAT3 Player::getPosition()
{
	return m_position;
}

XMVECTOR Player::getDirection()
{
	return m_direction;
}
