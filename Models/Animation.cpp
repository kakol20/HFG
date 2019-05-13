#include "Animation.h"


Animation::Animation()
{
	m_counter = 0;
	m_fps = 1;
	m_isAnimated = false;
	m_dtCumulative = 0.0f;
	m_nOfFrames = 8;
}

Animation::Animation(const Animation & copyAnimation)
{
	m_fps = copyAnimation.m_fps;
	m_counter = copyAnimation.m_counter;
	m_nOfFrames = copyAnimation.m_nOfFrames;
	m_isAnimated = copyAnimation.m_isAnimated;
	m_dtCumulative = copyAnimation.m_dtCumulative;

	int count = 1;

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		Mesh * temp = new Mesh;
		temp = copyAnimation.m_meshes[i];

		m_meshes[i] = temp;
		temp = nullptr;
		count++;
	}

	if (count > m_meshes.size())
	{
		for (size_t i = count; i < copyAnimation.m_meshes.size(); i++)
		{
			Mesh * temp = new Mesh;
			temp = copyAnimation.m_meshes[i];

			m_meshes.push_back(temp);
			temp = nullptr;
		}
	}
	else if (count < m_meshes.size())
	{
		m_meshes.erase(m_meshes.begin() + count - 1);
	}
}

Animation & Animation::operator=(const Animation & copyAnimation)
{
	// TODO: insert return statement here

	if (this == &copyAnimation) return *this;

	m_fps = copyAnimation.m_fps;
	m_counter = copyAnimation.m_counter;
	m_nOfFrames = copyAnimation.m_nOfFrames;
	m_isAnimated = copyAnimation.m_isAnimated;
	m_dtCumulative = copyAnimation.m_dtCumulative;

	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		if (*it == nullptr)
		{
			delete *it;
			*it = nullptr;
		}
	}

	/*for (size_t i = 0; i < copyAnimation.m_meshes.size(); i++)
	{
		Mesh * temp = new Mesh;
		temp =
	}*/
	for (auto it = copyAnimation.m_meshes.begin(); it != copyAnimation.m_meshes.end(); it++)
	{
		Mesh * temp = new Mesh;
		temp = *it;
		m_meshes.push_back(temp);

		temp = nullptr;
	}

	//m_meshes = copyAnimation.m_meshes;

	return *this;

}

Animation::~Animation()
{
	for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		if ((*it) != nullptr)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}
}

bool Animation::Load(const std::string & name, const int numberOfFrames, HRESULT & d3dResult, ID3D11Device * d3dDevice)
{
	m_nOfFrames = numberOfFrames;

	m_meshes.reserve((size_t)m_nOfFrames);

	for (int i = 0; i < m_nOfFrames; i++)
	{
		Mesh * tempMesh = new Mesh;
		std::string tempString = name;

		tempString = tempString + std::to_string(i + 1) + ".obj";

		if (!tempMesh->Init(tempString.c_str(), d3dResult, d3dDevice)) return false;

		m_meshes.push_back(tempMesh);

		tempMesh = nullptr;
	}

	return true;
}

void Animation::Update(float dt)
{
	//	m_dtCumulative += dt;
	//
	//	if (m_animate)
	//	{
	//		if (m_dtCumulative >= (1 / m_fps))
	//		{
	//			m_dtCumulative = 0.0f;
	//			m_frame++;
	//
	//			if (m_frame == m_maxFrames)
	//			{
	//				m_frame = 0;
	//			}
	//		}
	//	}

	m_dtCumulative += dt;

	if (m_isAnimated)
	{
		if (m_dtCumulative >= (1 / m_fps))
		{
			m_dtCumulative = 0.0f;
			m_counter++;
		}
	}
}

void Animation::SetFPS(float fps)
{
	m_fps = fps;
}

void Animation::SetIsAnimated(bool flag)
{
	m_isAnimated = flag;
}

Mesh * Animation::GetMesh()
{

	int frame = m_counter % m_nOfFrames;

	return m_meshes[frame];
}
