#ifndef ANIMATION_H

#define ANIMATION_H

#include <string>

#include"Dx11DemoBase.h"

#include "Mesh.h"
#include "Texture.h"

class Animation
{
public:
	Animation();
	~Animation();

	void setMeshFrames(Mesh* meshFrames[]);
	void setTexture(Texture* texture);

	Mesh* getCurrentFrame();
	Texture* getTexture();

	XMMATRIX getWorldMat();

	void setPosition(XMFLOAT3 position);
	void setDirection(XMVECTOR direction);
	void setScale(XMFLOAT3 scale);

	void setAnimate(bool animate);

	void moveForward(float speed);

	XMFLOAT3 getPosition();
	XMVECTOR getDirection();

	void updateWorldMat();

	void Update(float dt);

private:

	Mesh * m_mesh;
	Texture* m_texture;
	XMFLOAT3 m_position;

	Mesh* m_meshFrames[10];

	XMMATRIX m_worldMat;
	XMMATRIX m_translation;
	XMMATRIX m_rotation;
	XMMATRIX m_scale;

	XMVECTOR m_defaultForward;
	XMVECTOR m_defaultRight;
	XMVECTOR m_defaultUp;

	XMVECTOR m_direction;

	int m_frame;
	int m_maxFrames;
	float m_fps;
	float m_dtCumulative;

	bool m_animate;
};

#endif // !ANIMATION_H
