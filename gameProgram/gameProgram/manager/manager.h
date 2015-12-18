//*****************************************************************************
//
// メインマネージャー [manager.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MANAGER_H
#define _MY_MANAGER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Phase;
class Renderer;
class Sound;
class Debugproc;
class Keyboard;
class PadXManager;
class Road;

class Manager
{
public:
	Manager(void);
	~Manager(void);

	static bool Create(Manager** outPointer, HINSTANCE hInstance, HWND hWnd, bool windowFlg);
	bool Initialize(HINSTANCE hInstance, HWND hWnd, bool windowFlg);
	void Finalize(void);
	bool Update(void);
	void Draw(void);

	void CalculateFPS(DWORD frameCnt, DWORD curTime, DWORD FPSLastTime);

	static void nextPhase(Phase* nextPhase) {m_nextPhase = nextPhase;}
	static void score(int scoreBlue, int scoreRed) {m_scoreBlue = scoreBlue; m_scoreRed = scoreRed;}

	static int	scoreBlue(void) {return m_scoreBlue;}
	static int	scoreRed(void) {return m_scoreRed;}

private:
	int m_countFPS;

	Phase*			m_phase;
	static Phase*	m_nextPhase;

	static int m_scoreBlue;
	static int m_scoreRed;

	Renderer*	m_renderer;
	Sound*		m_sound;
	Debugproc*	m_debugproc;

	Keyboard*	 m_keyboard;
	PadXManager* m_padXManager;
	Road*		 m_road;
};

//=============================================================================
#endif

// [EOF]
