#ifndef D3D_H
#define D3D_H

#include "../stdafx.h"


class CD3D
{
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

public:
	CD3D();
	CD3D(const CD3D&);
	~CD3D();

	bool Initialize(int _screenWidth, int _screenHeight, bool _vsync, HWND _hwnd, bool _fullscreen, 
		float _screenDepth, float _screenNear);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();
	void GetVideoCardInfo(char*, int&);

	ID3D11Device* GetDevice()	{return m_pDevice;}
	ID3D11DeviceContext* GetDeviceContext() {return m_pDeviceContext;}

	D3DXMATRIX GetProjectionMatrix()  {return m_mProjMatrix;}
	D3DXMATRIX GetWorldMatrix()		{return m_mWorldMatrix;}
	D3DXMATRIX GetOrthoMatrix()		{return m_mOrthoMatrix;}



};

#endif
