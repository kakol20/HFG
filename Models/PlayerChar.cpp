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
