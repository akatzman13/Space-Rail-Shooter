#ifndef CMAINMENUSTATE_H
#define CMAINMENUSTATE_H
#include "IBaseState.h"


class CMainMenuState : public IBaseState
{
public:
	CMainMenuState(void);
	~CMainMenuState(void);

	///////////////////////////////////////////////////////////////////////
	// Function:	Enter
	// Parameters:  void
	// Return:      void
	// Purpose:		Enters the current state
	///////////////////////////////////////////////////////////////////////
	void Enter(void);

	///////////////////////////////////////////////////////////////////////
	// Function:	Exit
	// Parameters:  void
	// Return:      void
	// Purpose:		Exits current state
	///////////////////////////////////////////////////////////////////////
	void Exit(void);

	///////////////////////////////////////////////////////////////////////
	// Function:	StatePushed
	// Parameters:  void
	// Return:      void
	// Purpose:		Tells the current state that it has been lowered on the
	//				stack
	///////////////////////////////////////////////////////////////////////
	void StatePushed() {};

	///////////////////////////////////////////////////////////////////////
	// Function:	Update
	// Parameters:  float fElapsedTime
	// Return:      void
	// Purpose:		Updates the current state
	///////////////////////////////////////////////////////////////////////
	void Update(float _fElaspedTime);

	///////////////////////////////////////////////////////////////////////
	// Function:	Input
	// Parameters:  void
	// Return:      boolean
	// Purpose:		Gathers input of the current state
	//				If it returns false - break out of game
	///////////////////////////////////////////////////////////////////////
	bool Input(void);

	///////////////////////////////////////////////////////////////////////
	// Function:	Render
	// Parameters:  void
	// Return:      void
	// Purpose:		Renders the current state
	///////////////////////////////////////////////////////////////////////
	void Render();
};
#endif

