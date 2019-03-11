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

	m_fps = 12.0f;
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
	if (m_isAnimated)
	{
		if (m_animation == "walk")
		{
			return m_walkAnim[m_currFrame];
		}
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

	updateWorldMat();
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

	updateWorldMat();
}

void Player::update(float dt, XMFLOAT3 opponentPosition)
{
	XMVECTOR Opponent = XMLoadFloat3(&XMFLOAT3(opponentPosition.x, opponentPosition.y, opponentPosition.z));
	XMVECTOR Self = XMLoadFloat3(&XMFLOAT3(m_position.x, m_position.y, m_position.z));
	
	m_direction = XMVectorSet(XMVectorGetX(Opponent) - XMVectorGetX(Self)
		, 0.0f,
		XMVectorGetZ(Opponent) - XMVectorGetZ(Self),
		0.0f);
	// animation

	if (m_isAnimated)
	{
		m_dtCumulative += dt;
		if (m_dtCumulative >= (1 / m_fps))
		{
			m_dtCumulative = 0.0f;
			m_currFrame++;

			if (m_currFrame == 7)
			{
				m_currFrame = 0;
			}
		}
	}

	//m_direction = XMdVector3ClampLength(m_direction, 0.0f, 1.0f);
	m_direction = XMVector3Normalize(m_direction);

	m_angle = atan2(XMVectorGetX(m_direction), XMVectorGetZ(m_direction));

	updateWorldMat();
}

void Player::updateWorldMat()
{
	//m_scale = XMMatrixIdentity();
	m_translation = XMMatrixIdentity();
	m_worldMat = XMMatrixIdentity();
	m_rotation = XMMatrixIdentity();
	m_scaleMat = XMMatrixIdentity();


	/*m_scaleMat = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);*/
	m_scaleMat = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	/*XMVECTOR invertX = XMVectorSet(-1.0f*XMVectorGetX(m_direction), XMVectorGetY(m_direction), XMVectorGetZ(m_direction), 0.0f);

	XMVECTOR xAxis = XMVector3Cross(m_defaultUp, invertX);
	xAxis = XMVector3Normalize(xAxis);

	m_rotation = XMMatrixSet(
		XMVectorGetX(xAxis), XMVectorGetX(m_defaultUp), XMVectorGetX(invertX), 0.0f,
		XMVectorGetY(xAxis), XMVectorGetY(m_defaultUp), XMVectorGetY(invertX), 0.0f,
		XMVectorGetZ(xAxis), XMVectorGetZ(m_defaultUp), XMVectorGetZ(invertX), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/

	/*if (m_player1) 
	{
		m_rotation = XMMatrixRotationY(XM_PIDIV2);
	}
	else
	{
		m_rotation = XMMatrixRotationY(XM_PIDIV2 + XM_PI);
	}*/

	m_rotation = XMMatrixRotationY(m_angle);
	
	m_worldMat = m_scaleMat * m_rotation * m_translation;

	//m_worldMat =  m_translation;

	m_worldMat = XMMatrixTranspose(m_worldMat);
}

void Player::SetPlayer(bool player)
{
	m_player1 = player;
}

void Player::SetCharacter(Characters character)
{
	/*if (character == WOLF)
	{
		
	
	};

	else if (character == ROBOT)
	{
	
	};

	else if (character == KREMIT)
	{

	};

	else if (character == ZOMBIE)
	{

	};

	else if (character == ALIEN)
	{

	};

	else if (character == SKINNY)
	{

	};

	else if (character == PRAVEZ)
	{

	};*/
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

void Player::setRadius(float radius2)
{
	m_radius = radius2;
}

float Player::getRadius()
{
	return m_radius;
}
