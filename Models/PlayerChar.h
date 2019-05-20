#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H

#include"Dx11DemoBase.h"
#include "Mesh.h"
#include "Texture.h"
#include "Animation.h"

#include <string>
#include <math.h>



class Player
{
public:
	Player();
	~Player();

	void setMesh(Mesh * mesh);
	void setTexture(Texture* texture);

	Mesh* getMesh();
	Texture* getTexture();

	XMMATRIX getWorldMat();

	void setScale(XMFLOAT3 scale);
	//void setDirection(XMVECTOR direction);
	void setPosition(XMFLOAT3 position);

	void SetCharacter(Characters character);

	void moveForward(float dt, bool reverse);
	void moveRight(float dt, bool reverse);
	void update(float dt, XMFLOAT3 opponentPosition);
	void updateWorldMat();

	void setIsAnimated(bool animated);
	bool getIsAnimated();
	void setAnimation(const std::string & name);

	void setIdleMesh(Animation * animation);
	void setWalkMesh(Animation * animation);
	void setDeathMesh(Animation * animation);
	void setAttackMesh(Animation * animation);
	void setDamagedMesh(Animation * animation);

	void SetAlive(bool Alive) { m_Alive = Alive; }
	bool GetAlive() { return m_Alive; }

	float const GetAttack() { return m_attack; };
	void ApplyDamage(float damage);

	float const GetHealth() { return m_health; }
	void setFPS(float fps);

	void TogglePause() { m_isPaused = !m_isPaused; };
	void ToggleReverse() { m_reverse = !m_reverse; };

	void SetCurrentFrame(int currentFrame) { m_currFrame = currentFrame; };

	bool IsPaused() { return m_isPaused; };
	bool IsReversed() { return m_reverse; };

	void SetPlayer(bool player);
	//void SetCharacter(Characters character);
	XMFLOAT3 getPosition();
	XMVECTOR getDirection();
	
	void setRadius( float radius2);
	float getRadius();

protected:
	Mesh * m_mesh;
	Texture* m_texture;
	XMFLOAT3 m_position;
	XMFLOAT3 m_scale;

	Animation * m_idleAnim;
	Animation * m_walkAnim;
	Animation * m_deathAnim;
	Animation * m_attackAnim;
	Animation * m_damagedAnim;

	int m_currFrame;
	bool m_isAnimated;
	std::string m_animation;
	float m_dtCumulative;
	float m_fps;
	int m_maxFrames;

	XMMATRIX m_worldMat;
	XMMATRIX m_translation;
	XMMATRIX m_rotation;
	XMMATRIX m_scaleMat;

	//enum Animations { IDLE, WALK, ATTACK, DEATH, DAMAGED };

	XMVECTOR m_defaultForward;
	XMVECTOR m_defaultRight;
	XMVECTOR m_defaultUp;

	XMVECTOR m_direction;

	int m_Character;
	float m_health = 20.0f;
	float m_speed;
	float m_attack = 1.0f;
	float m_radius;
	bool m_player1 = true;
	bool m_Alive = true;

	float m_angle;

	bool m_isPaused;

	bool m_reverse;
};

#endif // !PLAYERCHAR_H
