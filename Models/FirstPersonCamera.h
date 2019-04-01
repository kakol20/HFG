#ifndef _FP_CAM_H_
#define _FP_CAM_H_

#include<xnamath.h>

#include "PlayerChar.h"

#include <vector>

#include <iostream>


class FirstPersonCamera
{
	public:
		FirstPersonCamera(void);
		~FirstPersonCamera(void);


		void SetDistance( float distance, float minDistance, float maxDistance );
		void SetRotation( float x, float y, float minY, float maxY );
		void SetTarget( XMFLOAT3& target );
		void SetPosition(float x, float y, float z);
		XMFLOAT3 GetPosition( );
		// void SetDirection(float x, float y, float z);
		void Move(float leftRight, float backFoward, float upDown);

		void ApplyZoom( float zoomDelta );
		void ApplyRotation( float yawDelta, float pitchDelta );
		XMMATRIX GetViewMatrix();

		// OWN  CODE
		void update(float dt, Player * Player1, Player * Player2);

		void snapPosition(Player * Player1, Player * Player2); // for moving the camera without smooth movement

		void setControllable(bool flag) { m_controllable = flag; };
		bool getControllable() { return m_controllable; };

		void setSpeed(float speed) { m_maxSpeed = speed; };

	private:
		XMFLOAT3 position_;
		XMFLOAT3 target_;
		XMFLOAT3 direction_;

		float distance_, minDistance_, maxDistance_;
		float xRotation_, yRotation_, yMin_, yMax_;

		XMVECTOR DefaultForward_ ;
		XMVECTOR DefaultRight_;
		XMVECTOR camForward_;
		XMVECTOR camRight_;

		XMMATRIX camView_;

		XMVECTOR camUp_;

		// own variables

		bool m_controllable;
		float m_distance; // distance to target
		float m_minDistance;

		float m_maxSpeed;
		float m_speedAc; // camera movement acceleration
		float m_curSpeed; // current speed

		float m_lookAtY; // y axis of what the camera is looking at - fixed
		float m_lookAngle; // angle the camera is looking down

		void m_moveCameraSameHeight(float dt, Player * Player1, Player * Player2, bool tween);
		void m_moveCameraTilted(float dt, Player * Player1, Player * Player2, bool tween);

		int m_currStep;

		float m_maxPoints;
		int m_maxSteps;
		bool m_isMoving;
		float m_tension;
		
		std::vector<XMVECTOR> m_steps;
		std::vector<XMVECTOR> m_points;

		void calculateSteps(float dt);

		void m_smoothMove(XMFLOAT3 & mid, XMFLOAT3 & targetPos, float dt);

		XMVECTOR m_previousMovement;
};

#endif