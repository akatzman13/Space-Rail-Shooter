#ifndef STDAFX_H
#define STDAFX_H

#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <list>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp")

using std::string;
using std::ostream;
using std::cout;
using std::endl;
using std::list;
using std::vector;
// Used to release DirecX memory
#define RELEASE(ref)			{if (ref) { ref->Release(); ref = nullptr;}}
// Deletes dynamically allocated memory and sets the pointer to null
#define SAFEDELETE(POINTER)			{ if (POINTER) { delete POINTER; POINTER = nullptr; } }

// Bitwise operations
#define TurnBitOn(Flag, Bit)		(Flag |=  (1<<Bit))
#define TurnBitOff(Flag, Bit)		(Flag &= ~(1<<Bit))	
#define ToggleBit(Flag, Bit)	(Flag ^=  (1<<Bit))	
#define ISBitOn(Flag,	Bit)		(Flag  &  (1<<Bit))

enum ConsoleInfo
{
	INFO = 0,
	WARNING = 1,
	ERRORS = 2,
};

#ifdef _DEBUG
#define OUTPUT(outputstatement)	{ std::wcout << outputstatement << endl;}
void WriteToConsole(const char* message, ConsoleInfo _info);
#endif

#ifndef _DEBUG
#include <vld.h>
#endif

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <d3dx11async.h>
#include <fstream>

#endif