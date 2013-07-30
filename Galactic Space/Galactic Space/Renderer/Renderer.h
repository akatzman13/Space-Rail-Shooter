#ifndef CRENDERER_H
#define CRENDERER_H

#include "../stdafx.h"
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

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
	CModel*  m_pModel;
	CShader* m_pColorShader;
};
#endif

