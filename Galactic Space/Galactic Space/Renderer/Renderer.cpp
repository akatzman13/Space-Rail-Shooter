#include "Renderer.h"



CRenderer::CRenderer(void)
{
	m_pD3D = NULL;
	m_pCamera = NULL;
	m_pBitmap = NULL;
	m_pTextureShader = NULL;
	moveX = 0;
	moving = false;
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
	m_nScreenWidth = _screenWidth;
	m_nScreenHeight = m_nScreenHeight;

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

	// Create the camera object.
	m_pCamera = new CCamera;
	if(!m_pCamera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_pBitmap = new CDxBitmap;
	if(!m_pBitmap)
	{
		return false;
	}

	// Initialize the model object.
	bresult = m_pBitmap->Initialize(m_pD3D->GetDevice(), _screenWidth, _screenHeight, L"../Galactic Space/Resources/Images/Lighthouse.jpg", 256,  256);
	if(!bresult)
	{
		WriteToConsole("Could not initialize bitmap", ERRORS);
		return false;
	}

	// Create the color shader object.
	m_pTextureShader = new CTextureShader;
	if(!m_pTextureShader)
	{
		return false;
	}

	// Initialize the color shader object.
	bresult = m_pTextureShader->Initialize(m_pD3D->GetDevice(), _hwnd);
	if(!bresult)
	{
		WriteToConsole("Could not initialize texture shader object", ERRORS);
		return false;
	}

	return true;
}

void CRenderer::Shutdown()
{
	// Release the color shader object.
	if(m_pTextureShader)
	{
		m_pTextureShader->Shutdown();
		delete m_pTextureShader;
		m_pTextureShader = 0;
	}

	// Release the model object.
	if(m_pBitmap)
	{
		m_pBitmap->Shutdown();
		delete m_pBitmap;
		m_pBitmap = 0;
	}

	// Release the camera object.
	if(m_pCamera)
	{
		SAFEDELETE(m_pCamera);
	}

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

	if(!moving)
	{
		moveX += 1.5f;
		if((moveX + m_pBitmap->GetBitmapWidth()) >= m_nScreenWidth)
			moving = true;
	}
	else if(moving)
	{
		moveX -= .5f;
		if(moveX <= m_nScreenHeight)
			moving = false;
	}

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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	viewMatrix = m_pCamera->GetViewMatrix();
	worldMatrix = m_pD3D->GetWorldMatrix();
	projectionMatrix = m_pD3D->GetProjectionMatrix();

	orthoMatrix = m_pD3D->GetOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	m_pD3D->TurnOffZBuffer();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pBitmap->Render(m_pD3D->GetDeviceContext(), (int)moveX, 100);

	// Render the bitmap with the texture shader.
	result = m_pTextureShader->Render(m_pD3D->GetDeviceContext(), m_pBitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_pBitmap->GetTexture());
	if(!result)
	{
		return false;
	}
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_pD3D->TurnOnZBuffer();

	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}