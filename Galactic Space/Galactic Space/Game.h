#ifndef CGAME_H
#define CGAME_H
#include "stdafx.h"
#include "States/IBaseState.h"
#include "States/GamePlayState.h"
#include "States/MainMenuState.h"
#include "Utilities/Timer.h"


class CGame
{
private:
	static CMainMenuState*		m_pMainMenuState;
	static CGamePlayState*		m_pGamePlayState;
	static IBaseState*			m_pCurrState;
	static IBaseState*			m_pNextState;
	static list<IBaseState*>    m_lsStateStack;
	static bool					m_bTransition;
    static int					m_nTypeOfChange;
public:
	static bool Initialize(); 
	static bool Update();
	static bool Render();
	static void PushState(IBaseState* _state);
	static bool Input(void);
	static void PopState(void);
	static void ChangeState(IBaseState* _state);
	static void LoadAssets();
	static void Shutdown();
	inline static IBaseState*		GetCurrentState()	{return m_pCurrState;}
	inline static IBaseState*		GetNextState()		{return m_pNextState;}
	inline static CMainMenuState*	GetMainMenuState()	{return m_pMainMenuState;}
	inline static CGamePlayState*	GetGamePlauState()	{return m_pGamePlayState;}
	inline static bool				GetIsTransition()	{return m_bTransition;}
	inline static int				GetTypeOfChange()	{return m_nTypeOfChange;}
	static CTimer	m_cGameTimer;
	CGame(void);
	~CGame(void);
};
#endif

