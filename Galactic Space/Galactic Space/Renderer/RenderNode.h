#ifndef CRENDERNODE_H
#define CRENDERNODE_H

#include "../stdafx.h"
#include "Shader.h"
#include "DxBitmap.h"

class CRenderNode
{
private:
	CShader*	m_pShader;
	CDxBitmap*  m_pBitmap;

public:
	CRenderNode(CShader* _shader, ID3D11Device*);
	~CRenderNode(void);
	bool Render();
	CShader* GetShader()	{return m_pShader;}
	CDxBitmap*  GetBitmap()		{return m_pBitmap;}

};

#endif
