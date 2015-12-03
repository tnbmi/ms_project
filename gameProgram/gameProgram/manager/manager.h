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

private:
	int m_countFPS;

	static Phase* m_phase;
	static Phase* m_nextPhase;

	Renderer*	m_renderer;
	Sound*		m_sound;
	Debugproc*	m_debugproc;

	Keyboard*	 m_keyboard;
	PadXManager* m_padXManager;
};

//=============================================================================
#endif

// [EOF]
