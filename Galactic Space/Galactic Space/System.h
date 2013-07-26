#ifndef CSYSTEM_H
#define CSYSTEM_H

#include "stdafx.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"

void WriteToConsole(const char* message);

class CSystem
{
public:
	CSystem(void);
	CSystem(const CSystem&);
	~CSystem(void);

	bool Init();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam);


private:
	bool Frame();
	void InitWindows(int& _screenWidth, int& _screenHeight);
	void ShutdownWindows();
	HANDLE consoleHandle;


	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	CRenderer* m_pRenderer;
	CInputManager* m_pInput;
};

static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam);

static CSystem* ApplicationHandle = 0;

#endif

