#include "InputManager.h"


CInputManager::CInputManager(void)
{
}


CInputManager::~CInputManager(void)
{
}

CInputManager::CInputManager(const CInputManager& other)
{
}

void CInputManager::Init()
{
	for(int i = 0; i < 256; i ++)
	{
		m_bkeys[i] = false;
	}

	return;
}

void CInputManager::KeyDown(unsigned int _key)
{
	// If a key is pressed then save that state in the key array.
	m_bkeys[_key] = true;
	return;
}


void CInputManager::KeyUp(unsigned int _key)
{
	// If a key is released then clear that state in the key array.
	m_bkeys[_key] = false;
	return;
}


bool CInputManager::IsKeyDown(unsigned int _key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_bkeys[_key];
}