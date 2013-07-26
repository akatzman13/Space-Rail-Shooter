#ifndef STDAFX_H
#define STDAFX_H

#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN 
#endif
#include <Windows.h>

#include <stdio.h>
#include <DbgHelp.h>
#pragma  comment(lib, "dbghelp")

#include <process.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using std::cout;
using std::string;
using std::endl;

// Used to release DirecX memory
#define RELEASE(ref)			{if (ref) { ref->Release(); ref = nullptr;}}
// Deletes dynamically allocated memory and sets the pointer to null
#define SAFEDELETE(POINTER)			{ if (POINTER) { delete POINTER; POINTER = nullptr; } }

// Bitwise operations
#define TurnBitOn(Flag, Bit)		(Flag |=  (1<<Bit))
#define TurnBitOff(Flag, Bit)		(Flag &= ~(1<<Bit))	
#define ToggleBit(Flag, Bit)	(Flag ^=  (1<<Bit))	
#define ISBitOn(Flag,	Bit)		(Flag  &  (1<<Bit))	

#endif