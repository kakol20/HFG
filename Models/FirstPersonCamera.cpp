

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

	m_distance = 40.0f;
	m_speed = 2.0f;
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
		XMFLOAT3 mid = { (Player1->getPosition().x + Player2->getPosition().x) / 2.0f, position_.y, (Player1->getPosition().z + Player2->getPosition().z) / 2.0f };

		XMFLOAT3 targetPosition = position_;

		//XMFLOAT3 tempDir = { , Player1->getPosition().y - Player2->getPosition().y,  };

		float tempX = Player1->getPosition().x - Player2->getPosition().x;
		float tempZ = Player1->getPosition().z - Player2->getPosition().z;

		XMFLOAT3 tempDir = { tempZ, 0.0f, -1 * tempX };

		XMVECTOR tempDirVect = XMLoadFloat3(&tempDir);
		tempDirVect = XMVector3Normalize(tempDirVect);

		/*XMVECTOR tempV = XMVectorSet(Player1->getPosition().x + Player2->getPosition().x, 0.0f, Player1->getPosition().z + Player2->getPosition().z, 0.0f);
		tempV = XMVector3Length(tempV);
		m_distance = XMVectorGetX(tempV);*/
		m_distance = pow(Player1->getPosition().x - Player2->getPosition().x, 2) + pow(Player1->getPosition().z - Player2->getPosition().z, 2);
		if (m_distance == 0.0f)
		{
			m_distance = 1.0f;
		}
		m_distance = sqrt(m_distance) + 10.0f;

		tempDirVect = tempDirVect * m_distance;

		tempDir.x = -1 * XMVectorGetX(tempDirVect);
		tempDir.z = -1 * XMVectorGetZ(tempDirVect);

		targetPosition.x = mid.x + tempDir.x;
		targetPosition.z = mid.z + tempDir.z;

		XMFLOAT3 deltaPos = { targetPosition.x - position_.x, targetPosition.y - position_.y, targetPosition.z - position_.z };

		XMVECTOR deltaPosV = XMLoadFloat3(&deltaPos);
		float distToTarget = pow(XMVectorGetX(deltaPosV), 2) + pow(XMVectorGetZ(deltaPosV), 2);
		if (distToTarget != 0) // does not need to move if the camera is already at the target position
		{
			distToTarget = sqrt(distToTarget);
			if (distToTarget < (m_speed * m_speed))
			{
				distToTarget = m_speed * m_speed;
			}
		
			deltaPosV = XMVector3Normalize(deltaPosV);

			float calSpeed = distToTarget / m_speed;
			deltaPosV = deltaPosV * calSpeed * dt;

			XMVECTOR newPosition = XMLoadFloat3(&position_) + deltaPosV;

			position_ = { XMVectorGetX(newPosition), XMVectorGetY(newPosition), XMVectorGetZ(newPosition) };

			XMVECTOR camTarget = XMLoadFloat3(&mid);
			//camTarget = XMVector3Normalize(camTarget);
			camView_ = XMMatrixLookAtLH(newPosition, camTarget, camUp_);
		}		
	}
}

void FirstPersonCamera::snapPosition(Player * Player1, Player * Player2)
{
	if (!m_controllable)
	{
		XMFLOAT3 mid = { (Player1->getPosition().x + Player2->getPosition().x) / 2.0f, position_.y, (Player1->getPosition().z + Player2->getPosition().z) / 2.0f };

		//XMFLOAT3 tempDir = { , Player1->getPosition().y - Player2->getPosition().y,  };

		float tempX = Player1->getPosition().x - Player2->getPosition().x;
		float tempZ = Player1->getPosition().z - Player2->getPosition().z;

		XMFLOAT3 tempDir = { tempZ, 0.0f, -1 * tempX };

		XMVECTOR tempDirVect = XMLoadFloat3(&tempDir);
		tempDirVect = XMVector3Normalize(tempDirVect);

		/*XMVECTOR tempV = XMVectorSet(Player1->getPosition().x + Player2->getPosition().x, 0.0f, Player1->getPosition().z + Player2->getPosition().z, 0.0f);
		tempV = XMVector3Length(tempV);
		m_distance = XMVectorGetX(tempV);*/
		m_distance = pow(Player1->getPosition().x - Player2->getPosition().x, 2) + pow(Player1->getPosition().z - Player2->getPosition().z, 2);
		if (m_distance == 0.0f)
		{
			m_distance = 1.0f;
		}
		m_distance = sqrt(m_distance) + 10.0f;

		tempDirVect = tempDirVect * m_distance;

		tempDir.x = -1 * XMVectorGetX(tempDirVect);
		tempDir.z = -1 * XMVectorGetZ(tempDirVect);

		position_.x = mid.x + tempDir.x;
		position_.z = mid.z + tempDir.z;

		XMVECTOR camTarget = XMLoadFloat3(&mid);
		//camTarget = XMVector3Normalize(camTarget);
		camView_ = XMMatrixLookAtLH(XMLoadFloat3(&position_), camTarget, camUp_);
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