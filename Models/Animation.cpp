#include "Animation.h"

Animation::Animation()
{
	m_frame = 0;
	m_maxFrames = 10;
	m_fps = m_maxFrames / 1.0f;
	//m_fps = 24.0f;

	m_defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	m_defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_direction = m_defaultForward;
	m_position = { 0.0f, 0.0f, 0.0f };

	m_scale = XMMatrixIdentity();
	m_scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	m_animate = false;

	updateWorldMat();
}

Animation::~Animation()
{
}

void Animation::setMeshFrames(Mesh* meshFrames[])
{
	for (int i = 0; i < m_maxFrames; i++)
	{
		m_meshFrames[i] = meshFrames[i];
	}

}

void Animation::setTexture(Texture * texture)
{
	m_texture = texture;
}

Mesh * Animation::getCurrentFrame()
{
	return m_meshFrames[m_frame];
}

Texture * Animation::getTexture()
{
	return m_texture;
}

XMMATRIX Animation::getWorldMat()
{
	return m_worldMat;
}

void Animation::setPosition(XMFLOAT3 position)
{
	m_translation = XMMatrixIdentity();

	m_position = position;

	m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	updateWorldMat();
}

void Animation::setDirection(XMVECTOR direction)
{
	m_direction = XMVector3Normalize(direction);
	//m_direction = XMVector3Normalize(m_defaultForward);

	updateWorldMat();
}

void Animation::setScale(XMFLOAT3 scale)
{
	m_scale = XMMatrixIdentity();

	m_scale = XMMatrixScaling(scale.x, scale.y, scale.z);

	updateWorldMat();
}

void Animation::setAnimate(bool animate)
{
	m_animate = true;
}

void Animation::moveForward(float speed)
{
	float xDir = XMVectorGetX(m_direction);
	float zDir = XMVectorGetZ(m_direction);

	m_position.x += xDir * speed;
	m_position.z += zDir * speed;

	updateWorldMat();
}

XMFLOAT3 Animation::getPosition()
{
	return m_position;
}

XMVECTOR Animation::getDirection()
{
	return m_direction;
}

void Animation::updateWorldMat()
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

void Animation::Update(float dt)
{
	m_dtCumulative += dt;

	if (m_animate)
	{
		if (m_dtCumulative >= (1 / m_fps))
		{
			m_dtCumulative = 0.0f;
			m_frame++;

			if (m_frame == m_maxFrames)
			{
				m_frame = 0;
			}
		}
	}

	// update frames
	
}
