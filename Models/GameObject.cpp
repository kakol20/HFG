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

	m_scale = XMMatrixScaling(scale.x, scale.y, scale.y);

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

	//d3dContext->UpdateSubresource
}

//void GameObject::draw(ID3D11DeviceContext* d3dContext, HRESULT & d3dResult, ID3D11Device * d3dDevice, const UINT * pStrides, const UINT * pOffset)
//{
//	d3dContext.ias
//}
