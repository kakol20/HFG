#ifndef ANIMATION_H

#define ANIMATION_H

#include <string>

#include"Dx11DemoBase.h"

#include <vector>

#include "Mesh.h"
//#include "Texture.h"

#include <string>
#include <algorithm>

class Animation
{
public:
	Animation();
	Animation(const Animation & copyAnimation);
	Animation & operator=(const Animation & copyAnimation);
	~Animation();

	bool Load(const std::string & name, const int numberOfFrames, HRESULT & d3dResult, ID3D11Device * d3dDevice);
	
	void Update(float dt);

	void SetFPS(float fps);
	void SetIsAnimated(bool flag);

	Mesh * GetMesh(int frame);

	int GetMaxFrames() const { return m_nOfFrames; };

private:

	int m_nOfFrames;
	//int m_frame;
	int m_counter;

	std::vector<Mesh *> m_meshes;
	
	float m_fps;
	float m_dtCumulative;

	bool m_isAnimated;
};

#endif // !ANIMATION_H
