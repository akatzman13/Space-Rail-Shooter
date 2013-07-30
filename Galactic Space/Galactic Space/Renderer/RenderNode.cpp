#include "RenderNode.h"


CRenderNode::CRenderNode(CShader* _shader)
{
	m_pModel = NULL;
	m_pShader = _shader;
}


CRenderNode::~CRenderNode(void)
{
	if(m_pModel)
	{
		delete m_pModel;
		m_pModel = NULL;
	}
	m_pShader = NULL;
}
