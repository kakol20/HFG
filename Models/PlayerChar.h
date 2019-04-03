#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H

#include"Dx11DemoBase.h"
#include "Mesh.h"
#include "Texture.h"

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

	void setIdleMesh(Mesh * meshFrames[]);
	void setWalkMesh(Mesh * meshFrames[]);
	void setDamagedMesh(Mesh * meshFrames[]);
	void setDeathMesh(Mesh * meshFrames[]);
	void setAttackMesh(Mesh* meshFrames[]);

	void SetAlive(bool Alive) { m_Alive = Alive; }
	bool GetAlive() { return m_Alive; }


	float const GetAttack() { return m_attack; };
	void ApplyDamage(float damage);

	float const GetHealth() { return m_health; }
	void setFPS(float fps) { m_fps = fps; }


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

	Mesh * m_idleAnim[8];
	Mesh * m_walkAnim[8];
	Mesh * m_attackAnim[8];
	Mesh * m_deathAnim[8];
	Mesh * m_damagedAnim[8];
	int m_currFrame;
	bool m_isAnimated;
	std::string m_animation;
	float m_dtCumulative;
	float m_fps;

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
};

#endif // !PLAYERCHAR_H
