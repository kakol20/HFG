

#include<d3d11.h>
#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(void): target_( XMFLOAT3( 0.0f, 0.0f, 0.0f ) ),
    position_( XMFLOAT3( 3.0f, 10.0f, -30.0f )), direction_( XMFLOAT3( 0.0f, 0.0f, 1.0f ) )
{
	SetDistance( 2.0f, 1.0f, 10.0f );
    SetRotation( 0.0f, 0.0f, -XM_PIDIV4/3, XM_PIDIV4/2 );
	//SetRotation( 0.0f, 0.0f, -XM_PIDIV4/4, XM_PIDIV4/4 );

	DefaultForward_ = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	DefaultRight_ = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);
	camForward_ = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	camRight_ = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

	//camPosition = XMVectorSet( 0.0f, 5.0f, -8.0f, 0.0f );
	//camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp_ = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	position_.y = 10.0f;

	// set true to move camera freely
	m_controllable = false;

	// 
	m_distance = 40.0f;
	m_minDistance = 25.0f;
	m_maxSpeed = 20.0f;
	//m_speedAc = 16.0f;
	m_speedAc = m_maxSpeed;
	m_curSpeed = 0.0f;

	m_lookAtY = 10.0f;
	m_lookAngle = 10.0f;
}


FirstPersonCamera::~FirstPersonCamera(void)
{
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
	position_.x = x;
	position_.y = y;
	position_.z = z;
}

void FirstPersonCamera::SetDistance( float distance, float minDistance, float maxDistance )
{
    distance_ = distance;
    minDistance_ = minDistance;
    maxDistance_ = maxDistance;

    if( distance_ < minDistance_ ) distance_ = minDistance_;
    if( distance_ > maxDistance_ ) distance_ = maxDistance_;
}


void FirstPersonCamera::SetRotation( float x, float y, float minY, float maxY )
{
    xRotation_ = x;
    yRotation_ = y;
    yMin_ = minY;
    yMax_ = maxY;

    if( yRotation_ < yMin_ ) yRotation_ = yMin_;
    if( yRotation_ > yMax_ ) yRotation_ = yMax_;
}


void FirstPersonCamera::SetTarget( XMFLOAT3& target )
{
    target_ = target;
}


void FirstPersonCamera::ApplyZoom( float zoomDelta )
{
    distance_ += zoomDelta;

    //if( distance_ < minDistance_ ) distance_ = minDistance_;
    //if( distance_ > maxDistance_ ) distance_ = maxDistance_;
}


void FirstPersonCamera::ApplyRotation( float yawDelta, float pitchDelta )
{
    xRotation_ += yawDelta;
    yRotation_ += pitchDelta;

    if( xRotation_ < yMin_ ) xRotation_ = yMin_;
    if( xRotation_ > yMax_ ) xRotation_ = yMax_;

	if( yRotation_ < -XM_2PI ) yRotation_ += XM_2PI;
    if( yRotation_ > XM_2PI ) yRotation_ -= XM_2PI;
}
void FirstPersonCamera::Move(float leftRight, float backFoward, float upDown)
{
	// ---------- NOT MODIFIED ----------
	XMMATRIX  camRotationMatrix = XMMatrixRotationRollPitchYaw(xRotation_, yRotation_, 0);
	XMVECTOR camTarget = XMVector3TransformCoord(DefaultForward_, camRotationMatrix );
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(yRotation_);

	camRight_ = XMVector3TransformCoord(DefaultRight_, RotateYTempMatrix);
	camUp_ = XMVector3TransformCoord(camUp_, RotateYTempMatrix);
	camForward_ = XMVector3TransformCoord(DefaultForward_, RotateYTempMatrix);

	XMVECTOR pos = XMLoadFloat3( &position_ );

	pos += leftRight*camRight_;
	pos += backFoward*camForward_;
	pos += upDown * camUp_;
	XMStoreFloat3( &position_, pos );
	//moveLeftRight = 0.0f;
	//moveBackForward = 0.0f;

	camTarget = pos + camTarget;	

	XMStoreFloat3( &direction_, camTarget );

	camView_ = XMMatrixLookAtLH( pos, camTarget, camUp_ );
}

XMFLOAT3 FirstPersonCamera::GetPosition( )
{
    return position_;
}

void FirstPersonCamera::update(float dt, Player * Player1, Player * Player2)
{
	// delay - make look more natural
	// - have smooth speed


	if (!m_controllable)
	{
		//m_moveCameraSameHeight(dt, Player1, Player2);

		m_moveCameraTilted(dt, Player1, Player2, true);
	}
}

void FirstPersonCamera::snapPosition(Player * Player1, Player * Player2)
{
	if (!m_controllable)
	{
		m_moveCameraTilted(0.0f, Player1, Player2, false);
	}
}

void FirstPersonCamera::m_moveCameraSameHeight(float dt, Player * Player1, Player * Player2, bool tween)
{
	XMFLOAT3 mid = { (Player1->getPosition().x + Player2->getPosition().x) / 2.0f, position_.y, (Player1->getPosition().z + Player2->getPosition().z) / 2.0f };

	XMFLOAT3 targetPosition = position_;

	//XMFLOAT3 tempDir = { , Player1->getPosition().y - Player2->getPosition().y,  };

	float tempX = Player1->getPosition().x - Player2->getPosition().x;
	float tempZ = Player1->getPosition().z - Player2->getPosition().z;

	XMFLOAT3 tempDir = { tempZ, 0.0f, -1 * tempX }; // create a perpendicular direction

	XMVECTOR tempDirVect = XMLoadFloat3(&tempDir);
	tempDirVect = XMVector3Normalize(tempDirVect); // make a vector with the same direction but with the length of 1

	m_distance = pow(Player1->getPosition().x - Player2->getPosition().x, 2) + pow(Player1->getPosition().z - Player2->getPosition().z, 2); // pythagoras' thereom
	if (m_distance == 0.0f)
	{
		m_distance = 1.0f;
	}
	m_distance = sqrt(m_distance);

	tempDirVect = tempDirVect * m_distance; // set distance

	tempDir.x = -1 * XMVectorGetX(tempDirVect); // flip direction on x and z axis
	tempDir.z = -1 * XMVectorGetZ(tempDirVect);

	targetPosition.x = mid.x + tempDir.x; // set the position the camera should be in
	targetPosition.z = mid.z + tempDir.z;
	
	if (tween)
	{
		m_smoothMove(mid, targetPosition, dt);
	}
	else
	{
		position_.x = targetPosition.x;
		position_.y = targetPosition.y;
		position_.z = targetPosition.z;
	}
}

void FirstPersonCamera::m_moveCameraTilted(float dt, Player * Player1, Player * Player2, bool tween)
{
	XMFLOAT3 mid = { (Player1->getPosition().x + Player2->getPosition().x) / 2.0f, m_lookAtY, (Player1->getPosition().z + Player2->getPosition().z) / 2.0f }; //  find the midpoint of the two players

	float tempX = Player1->getPosition().x - Player2->getPosition().x;
	float tempZ = Player1->getPosition().z - Player2->getPosition().z;

	float tempY = sqrt(tempX * tempX + tempZ * tempZ);
	tempY *= tan(XMConvertToRadians(m_lookAngle)); // calculate y axis of the camera based on the angle

	XMFLOAT3 tempDir = { tempZ, tempY, -1.0f * tempX };
	tempDir.x *= -1.0f;
	tempDir.z *= -1.0f;

	m_distance = sqrt(tempX * tempX + tempZ * tempZ) + 10.0f;

	if (m_distance < m_minDistance) // minimum distance
	{
		m_distance = m_minDistance;
	}

	XMVECTOR tempDirVect = XMLoadFloat3(&tempDir);
	tempDirVect = XMVector3Normalize(tempDirVect) * m_distance;

	XMFLOAT3 targetPosition = { mid.x + XMVectorGetX(tempDirVect), mid.y + XMVectorGetY(tempDirVect), mid.z + XMVectorGetZ(tempDirVect) };

	//float distToTarget = deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y + deltaPos.z * deltaPos.z;

	if (tween)
	{
		m_smoothMove(mid, targetPosition, dt);

	}
	else
	{
		position_.x = targetPosition.x;
		position_.y = targetPosition.y;
		position_.z = targetPosition.z;
	}

	XMVECTOR camTarget = XMLoadFloat3(&mid);
	XMVECTOR positionV = XMLoadFloat3(&position_);

	camView_ = XMMatrixLookAtLH(positionV, camTarget, camUp_);
}

void FirstPersonCamera::m_smoothMove(XMFLOAT3 & mid, XMFLOAT3 & targetPos, float dt)
{
	XMFLOAT3 deltaPos = { targetPos.x - position_.x, targetPos.y - position_.y, targetPos.z - position_.z };

	float distToTarget = deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y + deltaPos.z * deltaPos.z;

	if (distToTarget > 0.0f)
	{
		distToTarget = sqrt(distToTarget);

		XMVECTOR temp = XMLoadFloat3(&targetPos) - XMLoadFloat3(&mid);
		temp = XMVector3Length(temp);

		m_speedAc = m_maxSpeed - (XMVectorGetX(temp) - m_minDistance);
		m_speedAc /= 2.0f;

		if (m_speedAc < 1.0f)
		{
			m_speedAc = 1.0f;
		}

		if (m_curSpeed < m_maxSpeed)
		{
			m_curSpeed += m_speedAc * dt;
		}
		else
		{
			m_curSpeed = m_maxSpeed;
		}

		if (distToTarget > m_curSpeed * dt)
		{
			XMVECTOR deltaPosV = XMLoadFloat3(&deltaPos);
			deltaPosV = XMVector3Normalize(deltaPosV);

			deltaPosV *= m_curSpeed * dt;

			m_previousMovement = deltaPosV;

			position_.x += XMVectorGetX(deltaPosV);
			position_.y += XMVectorGetY(deltaPosV);
			position_.z += XMVectorGetZ(deltaPosV);
		}
		else
		{
			position_.x = targetPos.x;
			position_.y = targetPos.y;
			position_.z = targetPos.z;
		}
	}
	else
	{
		m_curSpeed = 0.0f;
	}	
}

XMMATRIX FirstPersonCamera::GetViewMatrix( )
{
    XMVECTOR zoom = XMVectorSet( 0.0f, 0.0f, distance_, 1.0f );
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw( xRotation_, -yRotation_, 0.0f );

    zoom = XMVector3Transform( zoom, rotation );

    XMVECTOR pos = XMLoadFloat3( &position_ );
    XMVECTOR lookAt = XMLoadFloat3( &target_ );
	XMVECTOR lookTo = XMLoadFloat3( &direction_ );

    //pos = lookAt + zoom;
	lookAt = pos - zoom;
    XMStoreFloat3( &position_, pos );

    XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f );
    up = XMVector3Transform( up, rotation );

	XMMATRIX viewMat = XMMatrixLookAtLH( pos, lookTo, up );
	//XMMATRIX viewMat = XMMatrixLookToLH( pos, lookAt, up );

    //return viewMat;

	return camView_;
}