#include "RenderNode.h"


CRenderNode::CRenderNode(CShader* _shader ,ID3D11Device* _device)
{
	//m_pBitmap->Initialize(_device);
	m_pShader = _shader;
}


CRenderNode::~CRenderNode(void)
{
	if(m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = 0;
	}
	m_pShader = NULL;
}

bool CRenderNode::Render()
{
	return false;
}

