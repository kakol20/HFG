

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
	m_maxSpeed = 40.0f;
	m_speedAc = 1.0f;
	//m_speedAc = m_maxSpeed;
	m_curSpeed = 0.0f;

	m_lookAtY = 10.0f;
	m_lookAngle = 10.0f;

	m_maxSteps = 5;
	m_maxPoints = 5;
	m_tension = 1;
	m_isMoving = false;

	m_points.reserve(m_maxPoints);
	m_points.reserve((m_maxSteps + 1) * m_maxPoints);
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

	/*if (m_isMoving)
	{
		m_maxPoints = m_maxPoints / (1 / 30.0f);
	}*/

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

	//XMFLOAT3 tempDir = { , Player1.getPosition().y - Player2.getPosition().y,  };

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

	bool isStill = XMVector3Equal(XMLoadFloat3(&position_), XMLoadFloat3(&targetPosition));

	if (tween && !isStill)
	{
		// old movement
		//m_smoothMove(mid, targetPosition, dt);
		
		// experimental movement
		m_moveSpline(mid, targetPosition, dt);
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

void FirstPersonCamera::m_moveSpline(XMFLOAT3 mid, XMFLOAT3 targetPosition, float dt)
{
	if (m_points.size() == (size_t)m_maxPoints && !m_isMoving)
	{
		m_isMoving = true;

		calculateSteps(dt);
	}
	else if (m_isMoving)
	{
		if (m_steps.empty())
		{
			m_isMoving = false;

			// delete points except last one

			auto it = m_points.end() - 1;

			m_points.erase(m_points.begin(), it);

			m_previousMovement = m_points[0];

			m_points.clear();
		}
		else
		{
			auto it = m_steps.end() - 1;
			//it--;

			XMFLOAT3 temp;
			temp.x = XMVectorGetX(*it);
			temp.y = XMVectorGetY(*it);
			temp.z = XMVectorGetZ(*it);

			m_smoothMove(mid, temp, dt);

			/*
			position_.x = XMVectorGetX(*it);
			position_.y = XMVectorGetY(*it);
			position_.z = XMVectorGetZ(*it);
			*/

			m_steps.erase(m_steps.end() - 1);
		}

	}
	else
	{
		m_points.push_back(XMLoadFloat3(&targetPosition));
	}
}

void FirstPersonCamera::calculateSteps(float dt)
{
	// TODO
	// Try spline interpolation
	// https://andrewhungblog.wordpress.com/2017/03/03/catmull-rom-splines-in-plain-english/
/*
	public static List GenerateSpline(List points, int stepsPerCurve = 3, float tension = 1)
	{
		List result = new List();

		for (int i = 0; i < points.Count - 1; i++)
		{
			Vector3 prev = i == 0 ? points[i] : points[i - 1];
			Vector3 currStart = points[i];
			Vector3 currEnd = points[i + 1];
			Vector3 next = i == points.Count - 2 ? points[i + 1] : points[i + 2];

			for (int step = 0; step <= stepsPerCurve; step++)
			{
				float t = (float)step / stepsPerCurve;
				float tSquared = t * t;
				float tCubed = tSquared * t;

				Vector3 interpolatedPoint =
					(-.5f * tension * tCubed + tension * tSquared - .5f * tension * t) * prev +
					(1 + .5f * tSquared * (tension - 6) + .5f * tCubed * (4 - tension)) * currStart +
					(.5f * tCubed * (tension - 4) + .5f * tension * t - (tension - 3) * tSquared) * currEnd +
					(-.5f * tension * tSquared + .5f * tension * tCubed) * next;

				result.Add(interpolatedPoint);
			}
		}

		return result;
	}
*/

	for (size_t i = 0; i < m_points.size() - 1; i++)
	{
		XMVECTOR prev = i == 0 ? m_previousMovement : m_points[i - 1];
		XMVECTOR currStart = m_points[i];
		XMVECTOR currEnd = m_points[i + 1];
		XMVECTOR next = i == m_points.size() - 2 ? m_points[i + 1] : m_points[i + 2];

		for (int step = 0; step <= m_maxSteps; step++)
		{
			float t = step / (float)m_maxSteps;
			float tSquared = t * t;
			float tCubed = tSquared * t;

			XMVECTOR interpolatedPoint = 
				(-0.5f * m_tension * tCubed + m_tension * tSquared - 0.5f * m_tension * t) * prev +
				(1.0f + 0.5f * tSquared * (m_tension - 6.0f) + 0.5f * tCubed * (4.0f - m_tension)) * currStart +
				(0.5f * tCubed * (m_tension - 4.0f) + 0.5f * m_tension * t - (m_tension - 3.0f) * tSquared) * currEnd +
				(-0.5f * m_tension * tSquared + 0.5f * m_tension * tCubed) * next;

			m_steps.push_back(interpolatedPoint);
		}
	}
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