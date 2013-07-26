#ifndef CINPUTMANAGER_H
#define CINPUTMANAGER_H

#include "../stdafx.h"
class CInputManager
{
public:
	CInputManager(void);
	CInputManager(const CInputManager&);
	~CInputManager(void);

	void Init();

	void KeyDown(unsigned int _key);
	void KeyUp(unsigned int _key);

	bool IsKeyDown(unsigned int _key);

private:
	bool m_bkeys[256];
};
#endif
