#include "stdafx.h"
#include "System.h"
LPCSTR applicationName = "Galactic Space";
HINSTANCE hInstance;
HWND hwnd;

////////////////////////USED CODE/////////////////////////////////////
// Taken from Wastland Mayhem
//////////////////////////////////////////////////////////////////////////
void InitializeApplication()
{
	//Get rid of sticky keys
	STICKYKEYS StartupStickyKeys = {sizeof(STICKYKEYS), 0};
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &StartupStickyKeys, 0);

	STICKYKEYS skOff = StartupStickyKeys;
	if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
	{
		skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
		skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
	}
}

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	CSystem* pSystem;
	bool result;

	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(static_cast<long>(-1));*/



	pSystem = new CSystem();
	if(!pSystem)
		return 0;

	result = pSystem->Init();	

	InitializeApplication();


	if(result)
		pSystem->Run();

	pSystem->ShutDown();
	delete pSystem;
	pSystem = NULL;

	return 0;
}