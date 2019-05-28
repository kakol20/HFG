#include "GameObject.h"

GameObject::GameObject()
{
	m_defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_direction = m_defaultForward;
	m_position = { 0.0f, 0.0f, 0.0f };

	m_scale = XMMatrixIdentity();
	m_scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
}

GameObject::~GameObject()
{
}

void GameObject::setMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

void GameObject::setTexture(Texture* texture)
{
	m_texture = texture;
}

Mesh * GameObject::getMesh()
{
	return m_mesh;
}

Texture * GameObject::getTexture()
{
	return m_texture;
}

XMMATRIX GameObject::getWorldMat()
{
	return m_worldMat;
}

void GameObject::setPosition(XMFLOAT3 position)
{
	m_translation = XMMatrixIdentity();

	m_position = position;

	m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	updateWorldMat();
}

void GameObject::setDirection(XMVECTOR direction)
{

	m_direction = XMVector3Normalize(direction);
	//m_direction = XMVector3Normalize(m_defaultForward);

	updateWorldMat();
}

void GameObject::setScale(XMFLOAT3 scale)
{
	m_scale = XMMatrixIdentity();

	m_scale = XMMatrixScaling(scale.x, scale.y, scale.z);

	updateWorldMat();
}

void GameObject::moveForward(float speed)
{
	
	float xDir = XMVectorGetX(m_direction);
	float zDir = XMVectorGetZ(m_direction);

	m_position.x += xDir * speed;
	m_position.z += zDir * speed;

	updateWorldMat();
}

XMFLOAT3 GameObject::getPosition()
{
	return m_position;
}

XMVECTOR GameObject::getDirection()
{
	return m_direction;
}

void GameObject::updateWorldMat()
{
	m_translation = XMMatrixIdentity();
	m_worldMat = XMMatrixIdentity();
	m_rotation = XMMatrixIdentity();

	m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	float angle = atan2(XMVectorGetX(m_direction), XMVectorGetZ(m_direction));

	m_rotation = XMMatrixRotationY(angle);

	m_worldMat = m_scale * m_rotation * m_translation;

	//m_worldMat =  m_translation;

	m_worldMat = XMMatrixTranspose(m_worldMat);

	//d3dContext->UpdateSubresource
}
