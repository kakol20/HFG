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

GameObject::GameObject(const GameObject & copyObject)
{
	/*Mesh* m_mesh;
	Texture* m_texture;
	XMFLOAT3 m_position;

	XMMATRIX m_worldMat;
	XMMATRIX m_translation;
	XMMATRIX m_rotation;
	XMMATRIX m_scale;

	XMVECTOR m_defaultForward;
	XMVECTOR m_defaultRight;
	XMVECTOR m_defaultUp;

	XMVECTOR m_direction;

	std::string m_id;*/

	//m_mesh = new Mesh;
	//m_texture = new Texture;
	*m_mesh = *copyObject.m_mesh;
	*m_texture = *copyObject.m_texture;

	m_position = copyObject.m_position;

	m_worldMat = copyObject.m_worldMat;
	m_translation = copyObject.m_translation;
	m_rotation = copyObject.m_rotation;
	m_scale = copyObject.m_rotation;

	m_defaultForward = copyObject.m_defaultForward;
	m_defaultRight = copyObject.m_defaultRight;
	m_defaultUp = copyObject.m_defaultUp;
	
	m_direction = copyObject.m_direction;

	m_id = copyObject.m_id;
}

GameObject & GameObject::operator=(const GameObject & copyObject)
{
	if (this == &copyObject) return *this;

	m_mesh = new Mesh;
	m_texture = new Texture;
	*m_mesh = *copyObject.m_mesh;
	*m_texture = *copyObject.m_texture;

	m_position = copyObject.m_position;

	m_worldMat = copyObject.m_worldMat;
	m_translation = copyObject.m_translation;
	m_rotation = copyObject.m_rotation;
	m_scale = copyObject.m_rotation;

	m_defaultForward = copyObject.m_defaultForward;
	m_defaultRight = copyObject.m_defaultRight;
	m_defaultUp = copyObject.m_defaultUp;

	m_direction = copyObject.m_direction;

	m_id = copyObject.m_id;

	return *this;
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

	try
	{
		m_worldMat = XMMatrixTranspose(m_worldMat);

	}
	catch (int e)
	{
			
	}
	//d3dContext->UpdateSubresource
}\


//Unhandled exception thrown : read access violation.
//**this** was 0xFFFFFFCF. occurred