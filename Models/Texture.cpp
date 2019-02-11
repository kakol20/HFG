#include "Texture.h"

Texture::Texture()
{
	m_colorMap = 0;
}

Texture::Texture(const Texture & copyTexture)
{
	m_colorMap = copyTexture.m_colorMap;
}

Texture & Texture::operator=(const Texture & copyTexture)
{
	if (this == &copyTexture) return *this;
	// TODO: insert return statement here

	m_colorMap = 0;
	m_colorMap = copyTexture.m_colorMap;

	return *this;
}

Texture::~Texture()
{
	m_colorMap = 0;
}

bool Texture::Init(char* name, HRESULT& d3dResult, ID3D11Device* d3dDevice)
{
	//m_name = new char;
	//*m_name = *name;

	d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice, name, 0, 0, &m_colorMap, 0);

	if (FAILED(d3dResult)) {
		return false;
	}

	return true;
}

bool Texture::setTexture(char * name, HRESULT & d3dResult, ID3D11Device * d3dDevice)
{
	d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice, name, 0, 0, &m_colorMap, 0);

	if (FAILED(d3dResult)) {
		return false;
	}

	return true;
}

void Texture::unloadTexture()
{
	if (m_colorMap) m_colorMap->Release();
}

ID3D11ShaderResourceView* const*  Texture::getColorMap()
{
	return &m_colorMap;
}
