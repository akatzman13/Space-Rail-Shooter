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
	D3D11_VIEWPORT					viewport;
	float							fov, screenAsp;

	// Store the vsync setting
	m_bVsync = _vsync;

	// Create a DX graphics interface factor
	hresult = CreateDXGIFactory(_uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(hresult))
	{
		WriteToConsole("Create factory failed!");
		return false;
	}
	else
	{
		WriteToConsole("Factory Successfully created!");
	}






	return true;
}

void CD3D::Shutdown()
{

}
	 
void CD3D::BeginScene(float, float, float, float)
{

}

void CD3D::EndScene()
{

}

ID3D11Device* CD3D::GetDevice()
{
	return NULL;
}

ID3D11DeviceContext* CD3D::GetDeviceContext()
{
	return NULL;
}

void CD3D::GetProjectionMatrix(D3DXMATRIX&)
{

}

void CD3D::GetWorldMatrix(D3DXMATRIX&)
{

}

void CD3D::GetOrthoMatrix(D3DXMATRIX&)
{

}

void CD3D::GetVideoCardInfo(char*, int&)
{

}