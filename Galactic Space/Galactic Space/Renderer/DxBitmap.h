#ifndef CDXBITMAP_H
#define CDXBITMAP_H
#include "../stdafx.h"
#include "Texture.h"

class CDxBitmap
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	ID3D11Buffer *m_pVertextBuff, *m_pIndexBuff;
	int m_nVertexCount, m_nIndexCount;
	CTexture* m_pTexture;
	int m_nScreenWidth, m_nScreenHeight;
	int m_nBitmapWidth, m_nBitmapHeight;
	int m_nPreviousPosX, m_nPreviousPosY;


public:
	CDxBitmap(void);
	CDxBitmap(const CDxBitmap&);
	~CDxBitmap(void);

	bool Initialize(ID3D11Device*, int, int, WCHAR*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount()		{return m_nIndexCount;}
	ID3D11ShaderResourceView* GetTexture() {return m_pTexture->GetTexture();}
	CTexture* GetCTexture() {return m_pTexture;}
	int GetBitmapWidth() {return m_nBitmapWidth;}
	int GetBitmapHeight() {return m_nBitmapHeight;}


private:
	bool InitializeBuffers(ID3D11Device* _device);
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);

};

#endif
