#include "System.h"

HANDLE consoleHandle;

CSystem::CSystem(void)
{
	m_pInput = NULL;
	m_pRenderer = NULL;
}

CSystem::CSystem(const CSystem&)
{
}


CSystem::~CSystem(void)
{
}

void WriteToConsole(const char* message, ConsoleInfo _info)
{
	if(_info == WARNING)
		SetConsoleTextAttribute(consoleHandle, 0x0E);
	else if(_info == INFO)
		SetConsoleTextAttribute(consoleHandle, 0x0A);
	else if(_info == ERRORS)
	{
		SetConsoleTextAttribute(consoleHandle, 0x0C);
		std::wcout << message << '\n';
		system("Pause");
		return;
	}

	std::wcout << message << '\n';
}

bool CSystem::Init()
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	AllocConsole();
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	FILE* file = NULL;
	freopen_s(&file, "CONOUT$", "w", stdout);
	// Initialize the width and height of the screen to 0 before it is assigned
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	// Init the windows api
	InitWindows(screenWidth, screenHeight);	
	WriteToConsole("Initialized Window", INFO);

	// Create a new input object used to handle user input
	m_pInput = new CInputManager();
	if(!m_pInput)
	{
		return false;
	}
	// Initialize the input object
	m_pInput->Init();
	WriteToConsole("Initialized Input", INFO);

	m_pRenderer = new CRenderer();
	if(!m_pRenderer)
	{
		return false;
	}
	// Initialize the renderer
	result = m_pRenderer->Init(screenWidth, screenHeight, m_hwnd);

	if(!result)
	{
		WriteToConsole("Renderer initialized with errors.", ERRORS);
		return false;
	}

	WriteToConsole("Renderer initialized successfully!", INFO);
	return true;	
}

void CSystem::ShutDown()
{
	if(m_pRenderer)
	{
		m_pRenderer->Shutdown();
		SAFEDELETE(m_pRenderer);
	}

	if(m_pInput)
	{
		SAFEDELETE(m_pInput);
	}

	ShutdownWindows();

	return;
}

void CSystem::Run()
{
	MSG msg;
	bool done, result;

	// Clear the Memory
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until quit message
	done = false;
	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			done = true;
		else
		{
			result = Frame();
			if(!result)
				done = true;
		}
	}
	return;
}

bool CSystem::Frame()
{
	bool result;

	// Check if user pressed escape
	if(m_pInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = m_pRenderer->Frame();
	if(!result)
		return false;

	return true;
}

LRESULT CALLBACK CSystem::MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
{
	switch(_umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_pInput->KeyDown((unsigned int)_wparam);
			return 0;
		}
		break;
		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_pInput->KeyUp((unsigned int)_wparam);
			return 0;
		}
		break;
		// Any other messages send to the default message handler as our application won't make use of them.
	default:
		{
			return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
		}
		break;
	}
}

void CSystem::InitWindows(int& _screenWidth, int& _screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Galactic Space";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	_screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	_screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		_screenWidth  = 800;
		_screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - _screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - _screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, _screenWidth, _screenHeight, NULL, NULL, m_hInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void CSystem::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)
{
	switch(_umsg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
		// Check if the window is being closed.
	case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
		break;
		// All other messages pass to the message handler in the system class.
	default:
		{
			return ApplicationHandle->MessageHandler(_hwnd, _umsg, _wparam, _lparam);
		}
		break;
	}
}