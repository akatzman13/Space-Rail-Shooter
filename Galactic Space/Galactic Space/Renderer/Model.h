#ifndef CMODEL_H
#define CMODEL_H
#include "../stdafx.h"

class CModel
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	ID3D11Buffer *m_pVertextBuff, *m_pIndexBuff;
	int m_nVertexCount, m_nIndexCount;

public:
	CModel(void);
	CModel(const CModel&);
	~CModel(void);

	bool Initialize(ID3D11Device* _device);
	void Shutdown();
	void Render(ID3D11DeviceContext* _deviceContext);
	
	int GetIndexCount()		{return m_nIndexCount;}

private:
	bool InitializeBuffers(ID3D11Device* _device);
};

#endif