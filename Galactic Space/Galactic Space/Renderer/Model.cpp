#include "Model.h"


CModel::CModel(void)
{
	m_pVertextBuff = NULL;
	m_pIndexBuff = NULL;
}


CModel::CModel(const CModel& other)
{

}

CModel::~CModel(void)
{

}

bool CModel::Initialize(ID3D11Device* _device)
{
	bool bresult;

	bresult = InitializeBuffers(_device);
	if(!bresult)
	{
		WriteToConsole("Failed to init buffers", ERRORS);
		return false;
	}
	
	WriteToConsole("Succeeded in writing buffers", INFO);
	return true;
}

void CModel::Shutdown()
{
	if(m_pIndexBuff)
		RELEASE(m_pIndexBuff);

	if(m_pVertextBuff)
		RELEASE(m_pVertextBuff);

	return;
}

void CModel::Render(ID3D11DeviceContext* _deviceContext)
{
	unsigned int stride, offset;

	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuff, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetIndexBuffer(m_pIndexBuff, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, 
	// in this case triangles.
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool CModel::InitializeBuffers(ID3D11Device* _device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_nVertexCount = 3;

	// Set the number of indices in the index array.
	m_nIndexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_nVertexCount];
	if(!vertices)
	{
		WriteToConsole("Creating vertex array failed", ERRORS);
		return false;
	}
	else
		WriteToConsole("Created vertex array", INFO);

	// Create the index array.
	indices = new unsigned long[m_nIndexCount];
	if(!indices)
	{
		WriteToConsole("Creating index array failed", ERRORS);
		return false;
	}
	else
		WriteToConsole("Created index array", INFO);

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_nVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertextBuff);
	if(FAILED(result))
	{
		WriteToConsole("Failed to create vertex buffer", ERRORS);
		return false;
	}
	else
		WriteToConsole("Create vertex buffer", INFO);


	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = _device->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuff);
	if(FAILED(result))
	{
		WriteToConsole("Failed to create index buffer", ERRORS);
		return false;
	}
	else
		WriteToConsole("Create index buffer", INFO);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

