#include "DxBitmap.h"


CDxBitmap::CDxBitmap(void)
{
	m_pVertextBuff = NULL;
	m_pIndexBuff = NULL;
	m_pTexture = NULL;
}


CDxBitmap::CDxBitmap(const CDxBitmap& other)
{

}

CDxBitmap::~CDxBitmap(void)
{

}

bool CDxBitmap::Initialize(ID3D11Device* _device, int _screenWidth, int _screenHeight, WCHAR* _textureFile, int _bitmapWidth, int _bitmapHeight)
{
	bool bresult;

	m_nScreenWidth = _screenWidth;
	m_nScreenHeight = _screenHeight;
	m_nBitmapWidth = _bitmapWidth;
	m_nBitmapHeight = _bitmapHeight;

	m_nPreviousPosX = -1;
	m_nPreviousPosY = -1;

	bresult = InitializeBuffers(_device);
	if(!bresult)
	{
		WriteToConsole("Failed to init buffers", ERRORS);
		return false;
	}

	m_pTexture = new CTexture();
	if(!m_pTexture)
	{
		WriteToConsole("Failed to create CTexture", ERRORS);
		return false;
	}

	bresult = m_pTexture->Initialize(_device, _textureFile);
	if(!bresult)
	{
		WriteToConsole("Failed to initialized CTexture", ERRORS);
	}


	WriteToConsole("Succeeded in writing buffers", INFO);
	return true;
}

void CDxBitmap::Shutdown()
{
	if(m_pIndexBuff)
		RELEASE(m_pIndexBuff);

	if(m_pVertextBuff)
		RELEASE(m_pVertextBuff);

	if(m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	return;
}

void CDxBitmap::Render(ID3D11DeviceContext* _deviceContext, int _posX, int _posY)
{
	unsigned int stride, offset;
	bool bresult;

	bresult = UpdateBuffers(_deviceContext, _posX, _posY);
	if(!bresult)
	{
		WriteToConsole("Failed to Update Buffer", ERRORS);
		return;
	}

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


bool CDxBitmap::InitializeBuffers(ID3D11Device* _device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Set the number of vertices in the vertex array.
	m_nVertexCount = 6;

	// Set the number of indices in the index array.
	m_nIndexCount = m_nVertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_nVertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_nIndexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_nVertexCount));

	// Load the index array with data.
	for(i=0; i<m_nIndexCount; i++)
	{
		indices[i] = i;
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_nVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
		return false;
	}


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
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool CDxBitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	// If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if((positionX == m_nPreviousPosX) && (positionY == m_nPreviousPosY))
	{
		return true;
	}


	// If it has changed then update the position it is being rendered to.
	m_nPreviousPosX = positionX;
	m_nPreviousPosY = positionY;


	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((m_nScreenWidth / 2) * -1) + (float)positionX;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)m_nBitmapWidth;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(m_nScreenHeight / 2) - (float)positionY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)m_nBitmapHeight;


	// Create the vertex array.
	vertices = new VertexType[m_nVertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);


	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(m_pVertextBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_nVertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(m_pVertextBuff, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}