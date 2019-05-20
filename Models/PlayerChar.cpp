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

	m_idleAnim = nullptr;
	m_walkAnim = nullptr;
	m_deathAnim = nullptr;
	m_attackAnim = nullptr;
	m_damagedAnim = nullptr;

	m_isPaused = false;
	m_reverse = false;
	m_isAnimated = false;

	m_maxFrames = 8;
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
			return m_walkAnim->GetMesh(m_currFrame);
		}
		else if (m_animation == "attack")
		{
			return m_attackAnim->GetMesh(m_currFrame);
		}
		else if (m_animation == "death")
		{
			return m_deathAnim->GetMesh(m_currFrame);
		}
		else if (m_animation == "damaged")
		{
			return m_damagedAnim->GetMesh(m_currFrame);
		}
		else
		{
			return m_idleAnim->GetMesh(m_currFrame);
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

void Player::SetCharacter(Characters character)
{
	switch (character)
	{
	case WOLF: { m_health = 30 ; }
			   break;
	default: {m_health = 20; }

	}
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

	if (m_walkAnim != nullptr) m_walkAnim->SetIsAnimated(m_isAnimated);
	if (m_idleAnim != nullptr) m_idleAnim->SetIsAnimated(m_isAnimated);
	if (m_deathAnim != nullptr) m_deathAnim->SetIsAnimated(m_isAnimated);
	if (m_attackAnim != nullptr) m_attackAnim->SetIsAnimated(m_isAnimated);
	if (m_damagedAnim != nullptr) m_damagedAnim->SetIsAnimated(m_isAnimated);

	if (m_walkAnim != nullptr) m_walkAnim->SetFPS(m_fps);
	if (m_idleAnim != nullptr) m_idleAnim->SetFPS(m_fps);
	if (m_deathAnim != nullptr) m_deathAnim->SetFPS(m_fps);
	if (m_attackAnim != nullptr) m_attackAnim->SetFPS(m_fps);
	if (m_damagedAnim != nullptr) m_damagedAnim->SetFPS(m_fps);
	
	if (m_walkAnim != nullptr) m_walkAnim->Update(dt);
	if (m_idleAnim != nullptr) m_idleAnim->Update(dt);
	if (m_deathAnim != nullptr) m_deathAnim->Update(dt);
	if (m_attackAnim != nullptr) m_attackAnim->Update(dt);
	if (m_damagedAnim != nullptr) m_damagedAnim->Update(dt);

	m_dtCumulative += dt;
	
	if (m_dtCumulative >= (1 / m_fps))
	{
		m_dtCumulative = 0.0f;

		if (!m_isPaused)
		{
			if (m_reverse)
			{
				m_currFrame--;
			}
			else
			{
				m_currFrame = m_currFrame + 1;
			}
		}

		if (m_currFrame >= m_maxFrames)
		{
			m_currFrame = 0;
		}

		if (m_currFrame < 0)
		{
			m_currFrame = m_maxFrames - 1;
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

	m_rotation = XMMatrixRotationY(m_angle);
	
	m_worldMat = m_scaleMat * m_rotation * m_translation;

	//m_worldMat =  m_translation;

	m_worldMat = XMMatrixTranspose(m_worldMat);
}

void Player::SetPlayer(bool player)
{
	m_player1 = player;
}

void Player::setIsAnimated(bool animated)
{
	m_isAnimated = animated;
}

bool Player::getIsAnimated()
{
	return m_isAnimated;
}

void Player::setAnimation(const std::string & name)
{
	m_animation = name;
}

void Player::setIdleMesh(Animation * animation)
{
	m_idleAnim = nullptr;
	m_idleAnim = animation;
}

void Player::setWalkMesh(Animation * animation)
{
	m_walkAnim = nullptr;
	m_walkAnim = animation;
}

void Player::setAttackMesh(Animation * animation)
{
	m_attackAnim = nullptr;
	m_attackAnim = animation;
}

void Player::setDeathMesh(Animation * animation)
{
	m_deathAnim = nullptr;
	m_deathAnim = animation;
}

void Player::setDamagedMesh(Animation * animation)
{
	m_damagedAnim = nullptr;
	m_damagedAnim = animation;

}

void Player::ApplyDamage(float damage)
{
	m_health -= damage;
	if (m_health <= 0.0f) 
	{ 
		m_Alive = false;
	} 
}

void Player::setFPS(float fps)
{
	m_fps = fps;

	if (m_idleAnim != nullptr)
	{
		m_idleAnim->SetFPS(fps);
	}

	if (m_walkAnim != nullptr)
	{
		m_walkAnim->SetFPS(fps);
	}

	if (m_deathAnim != nullptr)
	{
		m_deathAnim->SetFPS(fps);
	}

	if (m_attackAnim != nullptr)
	{
		m_attackAnim->SetFPS(fps);
	}

	if (m_damagedAnim != nullptr)
	{
		m_damagedAnim->SetFPS(fps);
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
