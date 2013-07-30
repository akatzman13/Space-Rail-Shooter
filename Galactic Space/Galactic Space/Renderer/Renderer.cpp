#include "Renderer.h"

CRenderer::CRenderer(void)
{
	m_pD3D = NULL;
	m_pCamera = NULL;
	m_pModel = NULL;
	m_pColorShader = NULL;
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

	// Create the camera object.
	m_pCamera = new CCamera;
	if(!m_pCamera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_pModel = new CModel;
	if(!m_pModel)
	{
		return false;
	}

	// Initialize the model object.
	bresult = m_pModel->Initialize(m_pD3D->GetDevice());
	if(!bresult)
	{
		WriteToConsole("Could not initialize model", ERRORS);
		return false;
	}

	// Create the color shader object.
	m_pColorShader = new CShader;
	if(!m_pColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	bresult = m_pColorShader->Initialize(m_pD3D->GetDevice(), _hwnd);
	if(!bresult)
	{
		WriteToConsole("Could not initialize color shader object", ERRORS);
		return false;
	}

	return true;
}

void CRenderer::Shutdown()
{
	// Release the color shader object.
	if(m_pColorShader)
	{
		m_pColorShader->Shutdown();
		delete m_pColorShader;
		m_pColorShader = 0;
	}

	// Release the model object.
	if(m_pModel)
	{
		m_pModel->Shutdown();
		delete m_pModel;
		m_pModel = 0;
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
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	viewMatrix = m_pCamera->GetViewMatrix();
	worldMatrix = m_pD3D->GetWorldMatrix();
	projectionMatrix = m_pD3D->GetProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pModel->Render(m_pD3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_pColorShader->Render(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}


	// Present the rendered scene to the screen.
	m_pD3D->EndScene();

	return true;
}
