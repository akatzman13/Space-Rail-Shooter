#ifndef CMAINMENUSTATE_H
#define CMAINMENUSTATE_H
#include "IBaseState.h"


class CMainMenuState : public IBaseState
{
public:
	CMainMenuState(void);
	~CMainMenuState(void);


	void Enter(void);
	void Exit(void);
	void StatePushed() {};
	void Update(float _fElaspedTime);
	bool Input(void);
	void Render();
};
#endif

