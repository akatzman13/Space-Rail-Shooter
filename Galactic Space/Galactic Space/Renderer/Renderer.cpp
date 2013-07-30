#include "Renderer.h"

CRenderer::CRenderer(void)
{
	m_pD3D = NULL;
}

CRenderer::CRenderer(const CRenderer& other)
{

}


CRenderer::~CRenderer(void)
{
}


bool CRenderer::Init(int _screenWidth, int _screenHeight, HWND _hwnd)
{
	bool bresult;

	// Create the Direct3D object.
	m_pD3D = new CD3D();
	if(!m_pD3D)
	{
		WriteToConsole("Could not create CD3D object", ERRORS);
		return false;
	}
	else
	{
		WriteToConsole("Success in creating CD3D object", INFO);
	}

	// Initialize the Direct3D object.
	bresult = m_pD3D->Initialize(_screenWidth, _screenHeight, VSYNC_ENABLED, _hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!bresult)
	{
		WriteToConsole("Errors when Initializing CD3D class...", ERRORS);
		return false;
	}
	else
	{
		WriteToConsole("Success in Initializing CD3D class!", INFO);
	}
	return true;
}

void CRenderer::Shutdown()
{
	if(m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D = NULL;
	}
	return;
}

bool CRenderer::Frame()
{
	bool bresult;

	bresult = Render();
	if(!bresult)
	{
		WriteToConsole("Error while rendering...", ERRORS);
		return false;
	}	
	return true;
}

bool CRenderer::Render()
{
	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}
