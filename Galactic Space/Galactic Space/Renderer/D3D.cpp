#include "D3D.h"

void WriteToConsole(const char* message);

CD3D::CD3D(void)
{
	m_pSwapChain = NULL;
	m_pDevice = NULL;
	m_pDeviceContext = NULL;
	m_pRenderTargetView = NULL;
	m_pDepthStencilBuffer = NULL;
	m_pDepthStencilState = NULL;
	m_pDepthStencilView = NULL;
	m_pRasterState = NULL;

	m_pDepthStencilState = NULL;
}

CD3D::CD3D(const CD3D& other)
{
}


CD3D::~CD3D(void)
{
}

bool CD3D::Initialize(int _screenWidth, int _screenHeight, bool _vsync, HWND _hwnd, 
	bool _fullscreen, float _screenDepth, float _screenNear)
{
	HRESULT							hresult;
	IDXGIFactory*					factory;
	IDXGIAdapter*					adapter;
	IDXGIOutput*					adapterOutput;
	unsigned int					numModes, i, numerator, denominator, stringLen;
	DXGI_MODE_DESC*					displayModeList;
	DXGI_ADAPTER_DESC				adapterDesc;
	int								nError;
	DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	D3D_FEATURE_LEVEL				featureLevel;
	ID3D11Texture2D*				pBackBuffer;
	D3D11_TEXTURE2D_DESC			depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	D3D11_RASTERIZER_DESC			rasterDesc;
	D3D11_VIEWPORT					viewport;
	float							fov, screenAsp;
	D3D11_DEPTH_STENCIL_DESC		depthDisabledStencilDesc;

	// Store the vsync setting
	m_bVsync = _vsync;

	// Create a DX graphics interface factor
	hresult = CreateDXGIFactory(_uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(hresult))
	{
		WriteToConsole("Create factory failed...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Factory Successfully created!", INFO);

	// Use the factory to create an adapter for the primary graphics interface (video card).
	hresult = factory->EnumAdapters(0, &adapter);
	if(FAILED(hresult))
	{
		WriteToConsole("Enum adapters failed...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Enum adapters Succeeded!", INFO);

	// Use the factor to create an adapter for the video card
	hresult = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(hresult))
	{
		WriteToConsole("Enum Outputs failed...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Enum Outputs Succeeded!", INFO);


	// Get the number of modes that fit the DX_R8G9B9A9_UNORM display
	hresult = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes,NULL);
	if(FAILED(hresult))
	{
		WriteToConsole("GetDispalyModeList failed...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Get number of modes Succeeded!", INFO);

	// Create a list to hold all possible display modes
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		WriteToConsole("Display mode list failed...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Display mode list Succeeded!", INFO);

	// Now fill the display mode list structure
	hresult = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(hresult))
	{
		WriteToConsole("GetDispalyModeList failed to fill the display mode structure...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Get number of modes Succeeded to fill the display mode structure!", INFO);


	// Now go through all the modes and find the one that matched the screen width and height
	for(i = 0; i < numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)_screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)_screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	CHAR buff[200];
	sprintf_s(buff, "Found refresh rate Numerator: %d & Denominator: %d", numerator, denominator);
	WriteToConsole(buff, INFO);

	// Get adapter description
	hresult = adapter->GetDesc(&adapterDesc);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to get adapter description...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in getting adapter description!", INFO);

	// Store dedicated video card memory
	m_nVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to car array
	nError = wcstombs_s(&stringLen, m_cVideoCardDescription, 128, adapterDesc.Description, 128);
	if(nError != 0)
	{
		WriteToConsole("Failed to convert the video card name...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in converting the video card name!", INFO);

	// Release the display mode, adapter output, adapter, and factory
	SAFEDELETE(displayModeList);
	RELEASE(adapterOutput);
	RELEASE(adapter);
	RELEASE(factory);

	// Initialize the swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to single back buffer
	swapChainDesc.BufferCount = 1;

	// Set the width and height of back buffer
	swapChainDesc.BufferDesc.Width = _screenWidth;
	swapChainDesc.BufferDesc.Height = _screenHeight;

	// Set regular 32-bit surface for back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	// Set refresh rate of back buffer
	if(m_bVsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render
	swapChainDesc.OutputWindow = _hwnd;

	// Turn multi sampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed
	if(_fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scalling
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Dont set advanced flags
	swapChainDesc.Flags = 0;

	// Set feature level
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chaning, device and context
	hresult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create the swap Chaining, device, and device context...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in creating the swap Chaining, device, and device context...", INFO);

	// Get back buffer pointer
	hresult = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to get back buffer...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in getting back buffer!", INFO);

	// Create the render target
	hresult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create render target view...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in creating render target view!", INFO);

	// Release point to back buffer
	RELEASE(pBackBuffer);

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = _screenWidth;
	depthBufferDesc.Height = _screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hresult = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create depth buffer texture...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in creating depth buffer texture!", INFO);


	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hresult = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create depth stencil state...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in creating depth stencil state!", INFO);

	// Set the depth stencil state.
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hresult = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create depth stencil view...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in creating depth stencil view!", INFO);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hresult = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create raster state...", ERRORS);
		return false;
	}
	else
		WriteToConsole("Succeeded in creating raster state!", INFO);

	// Now set the rasterizer state.
	m_pDeviceContext->RSSetState(m_pRasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)_screenWidth;
	viewport.Height = (float)_screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fov = (float)D3DX_PI / 4.0f;
	screenAsp = (float)_screenWidth / (float)_screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&m_mProjMatrix, fov, screenAsp, _screenNear, _screenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&m_mWorldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&m_mOrthoMatrix, (float)_screenWidth, (float)_screenHeight, _screenNear, _screenDepth);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	hresult = m_pDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pdepthDisabledStencilState);
	if(FAILED(hresult))
	{
		WriteToConsole("Failed to create depth stencil state", ERRORS);
		return false;
	}


	return true;
}

void CD3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	if(m_pRasterState)
	{
		RELEASE(m_pRasterState);
	}

	if(m_pDepthStencilView)
	{
		RELEASE(m_pDepthStencilView);
	}

	if(m_pDepthStencilState)
	{
		RELEASE(m_pDepthStencilState)
	}

	if(m_pDepthStencilBuffer)
	{
		RELEASE(m_pDepthStencilBuffer);
	}

	if(m_pRenderTargetView)
	{
		RELEASE(m_pRenderTargetView);
	}

	if(m_pDeviceContext)
	{
		RELEASE(m_pDeviceContext);
	}

	if(m_pDevice)
	{
		RELEASE(m_pDevice);
	}

	if(m_pSwapChain)
	{
		RELEASE(m_pSwapChain);
	}

	if(m_pdepthDisabledStencilState)
	{
		m_pdepthDisabledStencilState->Release();
		m_pdepthDisabledStencilState = NULL;
	}
	return;
}
	 
void CD3D::BeginScene(float _red, float _green, float _blue, float _alpha)
{
	float color[4];
	// Setup the color to clear the buffer to.
	color[0] = _red;
	color[1] = _green;
	color[2] = _blue;
	color[3] = _alpha;

	// Clear the back buffer.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// Clear the depth buffer.
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void CD3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_bVsync)
	{
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
	}

	return;
}

void CD3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_cVideoCardDescription);
	memory = m_nVideoCardMemory;
	return;
}

void CD3D::TurnOnZBuffer()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	return;
}

void CD3D::TurnOffZBuffer()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pdepthDisabledStencilState, 1);
}