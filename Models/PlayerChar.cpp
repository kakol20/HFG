#include "PlayerChar.h"

Player::Player()
{
	m_defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_direction = m_defaultForward;
	m_position = { 0.0f, 0.0f, 0.0f };

	m_scale = XMMatrixIdentity();
	m_scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	m_worldMat = XMMatrixIdentity();
	m_translation = XMMatrixIdentity();
	m_rotation = XMMatrixIdentity();
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
	m_scale = XMMatrixIdentity();

	m_scale = XMMatrixScaling(scale.x, scale.y, scale.y);

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
	float xDir = XMVectorGetX(m_direction);
	float zDir = XMVectorGetZ(m_direction);

	if (reverse) // boolean for checking to move in the reverse direction
	{
		xDir *= -1;
		zDir *= -1;
	}

	m_position.x += xDir;
	m_position.y += zDir;

	updateWorldMat();
}

void Player::moveRight(float dt, bool reverse)
{
	float xDir = -1.0f * XMVectorGetZ(m_direction);
	float zDir = XMVectorGetX(m_direction);

	if (reverse) // boolean for checking to move in the reverse direction
	{
		xDir *= -1;
		zDir *= -1;
	}

	m_position.x += xDir;
	m_position.y += zDir;

	updateWorldMat();
}

void Player::update(XMFLOAT3 opponentPostion)
{
	// nothing for now
}

void Player::updateWorldMat()
{
	m_translation = XMMatrixIdentity();
	m_worldMat = XMMatrixIdentity();
	m_rotation = XMMatrixIdentity();

	m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	XMVECTOR invertX = XMVectorSet(-1.0f*XMVectorGetX(m_direction), XMVectorGetY(m_direction), XMVectorGetZ(m_direction), 0.0f);

	XMVECTOR xAxis = XMVector3Cross(m_defaultUp, invertX);
	xAxis = XMVector3Normalize(xAxis);

	m_rotation = XMMatrixSet(
		XMVectorGetX(xAxis), XMVectorGetX(m_defaultUp), XMVectorGetX(invertX), 0.0f,
		XMVectorGetY(xAxis), XMVectorGetY(m_defaultUp), XMVectorGetY(invertX), 0.0f,
		XMVectorGetZ(xAxis), XMVectorGetZ(m_defaultUp), XMVectorGetZ(invertX), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	m_worldMat = m_scale * m_rotation * m_translation;

	//m_worldMat =  m_translation;

	m_worldMat = XMMatrixTranspose(m_worldMat);
}

XMFLOAT3 Player::getPosition()
{
	return m_position;
}

XMVECTOR Player::getDirection()
{
	return m_direction;
}
