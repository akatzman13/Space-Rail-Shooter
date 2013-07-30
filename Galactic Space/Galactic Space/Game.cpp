#include "Game.h"

CMainMenuState*			CGame::m_pMainMenuState			= NULL;
CGamePlayState*			CGame::m_pGamePlayState			= NULL;
IBaseState*				CGame::m_pCurrState				= NULL;
list<IBaseState*>		CGame::m_lsStateStack;
CTimer					CGame::m_cGameTimer;
IBaseState*				CGame::m_pNextState				= NULL;
bool					CGame::m_bTransition			= false;
int						CGame::m_nTypeOfChange			= 0;
CGame::CGame(void)
{
}


CGame::~CGame(void)
{
}

bool CGame::Initialize()
{	
	m_pMainMenuState = new CMainMenuState();
	m_pGamePlayState = new CGamePlayState();
	LoadAssets();

	m_cGameTimer.Init();
	m_cGameTimer.Update();
	m_pCurrState = m_pMainMenuState;
	m_pCurrState->Enter();
	m_lsStateStack.push_front(m_pCurrState);

	return true;
}

void CGame::LoadAssets()
{
	return;
}

void CGame::Shutdown()
{
	SAFEDELETE(m_pGamePlayState);
	SAFEDELETE(m_pMainMenuState);
}

bool CGame::Update()
{
	m_cGameTimer.Update();
	return true;
}

bool CGame::Input()
{
	return true;
}

bool CGame::Render()
{
	return true;
}

void CGame::PushState(IBaseState * pState)
{
	m_pCurrState->StatePushed();
	m_pCurrState = pState;
	m_pCurrState->Enter();
	m_lsStateStack.push_front(m_pCurrState);
	m_cGameTimer.Update();
}


void CGame::PopState(void)
{
	m_pCurrState->Exit();
	m_lsStateStack.pop_front();

	if (m_lsStateStack.size() > 0)
		m_pCurrState = m_lsStateStack.front();
	m_cGameTimer.Update();
}

void CGame::ChangeState(IBaseState * pState)
{	
	m_pNextState = pState;
	m_nTypeOfChange = 2;
	m_bTransition = true;
	m_cGameTimer.Update();
}