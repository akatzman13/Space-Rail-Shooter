#ifndef CRENDERER_H
#define CRENDERER_H

#include "../stdafx.h"
#include "D3D.h"
#include "Camera.h"
#include "DxBitmap.h"
#include "TextureShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class CRenderer
{
public:
	CRenderer(void);
	CRenderer(const CRenderer&);
	~CRenderer(void);

	bool Init(int _screenWidth, int _screenHeight, HWND _hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	CD3D*	m_pD3D;
	CCamera* m_pCamera;
	CDxBitmap* m_pBitmap;
	CTextureShader* m_pTextureShader;
	float moveX;
	bool  moving;
	int m_nScreenWidth, m_nScreenHeight;
	

};
#endif

