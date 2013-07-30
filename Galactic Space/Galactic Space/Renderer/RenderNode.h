#ifndef CRENDERNODE_H
#define CRENDERNODE_H

#include "../stdafx.h"
#include "Model.h"
#include "Shader.h"

class CRenderNode
{
private:
	CShader*	m_pShader;
	CModel*		m_pModel;

public:
	CRenderNode(CShader* _shader);
	~CRenderNode(void);
	CShader* GetShader()	{return m_pShader;}
	CModel*  GetModel()		{return m_pModel;}

};

#endif
