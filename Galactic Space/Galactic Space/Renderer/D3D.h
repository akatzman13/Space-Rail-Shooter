#ifndef D3D_H
#define D3D_H

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

class CD3D
{
public:
	CD3D();
	CD3D(const CD3D&);
	~CD3D();

	bool Initialize(int _screenWidth, int _screenHeight, bool _vsync, HWND _hwnd, bool _fullscreen, 
		float _screenDepth, float _screenNear);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool m_bVsync;
	int m_nVideoCardMemory;
	char m_cVideoCardDescription[128];
	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;
	D3DXMATRIX m_mProjMatrix;
	D3DXMATRIX m_mWorldMatrix;
	D3DXMATRIX m_mOrthoMatrix;
};

#endif
