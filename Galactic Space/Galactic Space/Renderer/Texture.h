#ifndef _CTEXTURE_H
#define _CTEXTURE_H
#include "../stdafx.h"

class CTexture
{
private:
	ID3D11ShaderResourceView* m_texture;

public:
	CTexture(void);
	CTexture(const CTexture&);
	~CTexture(void);

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture() {return m_texture;}
};

#endif
