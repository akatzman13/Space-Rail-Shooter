#ifndef IBASESTATE_H
#define IBASESTATE_H
#include "../stdafx.h"

class IBaseState
{
private:
	// Enum for state ID
	int m_eStateId;

public:

	virtual void Enter(void) = 0;	
	virtual void Exit(void) = 0;
	virtual void StatePushed() = 0;
	virtual void Update	(float _fElaspedTime) = 0;
	virtual bool Input	(void) = 0;
	virtual void Render	() = 0;

	// ACCESSORS & MUTATORS
	int GetID(void) { return m_eStateId; }
	void SetID(int nID) { m_eStateId = nID; }
};

#endif