#ifndef TEXTURE_H
#define TEXTURE_H

#include"Dx11DemoBase.h"

class Texture {

public:
	Texture();
	Texture(const Texture& copyTexture);
	Texture& operator=(const Texture& copyTexture);
	~Texture();

	bool Init(char* name, HRESULT&  d3dResult, ID3D11Device* d3dDevice);

	void unloadTexture();

	//ID3D11ShaderResourceView* getColorMap() const;

	//void setShaderResource(ID3D11DeviceContext* d3dContext);
	ID3D11ShaderResourceView* const* getColorMap();

private:

	ID3D11ShaderResourceView* m_colorMap;
};

#endif // !TEXTURE_H
