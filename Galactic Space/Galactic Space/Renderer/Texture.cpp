#include "Texture.h"


CTexture::CTexture(void)
{
	m_texture = NULL;
}

CTexture::CTexture(const CTexture& other)
{

}

CTexture::~CTexture(void)
{

}

bool CTexture::Initialize(ID3D11Device* _device, WCHAR* filename)
{
	HRESULT hresult = D3DX11CreateShaderResourceViewFromFile(_device, filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(hresult))
	{
		char buff[200];
		sprintf_s(buff, "Failed to initialize shader file %s", filename);
		WriteToConsole(buff, ERRORS);
		return false;
	}
	char buff[200];
	sprintf_s(buff, "Succeeded in initializing shader file %s", filename);
	WriteToConsole(buff, INFO);
	return true;
}

void CTexture::Shutdown()
{
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	return;
}

