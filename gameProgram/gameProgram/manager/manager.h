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
class MainImport;
class FbxTexImport;
class Load;

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

	void CalculateFPS(DWORD frameCnt, DWORD curTime, DWORD FPSLastTime, DWORD updateTime, DWORD drawTime);

	static void nextPhase(Phase* nextPhase) {m_nextPhase = nextPhase;}
	static void score(int scoreBlue, int scoreRed) {m_scoreBlue = scoreBlue; m_scoreRed = scoreRed;}

	static int	scoreBlue(void) {return m_scoreBlue;}
	static int	scoreRed(void) {return m_scoreRed;}
	static MainImport	*mainImport(void) { return m_mainImport; }
	static FbxTexImport	*fbxTexImport(void) { return m_fbxTexImport; }

private:
	int m_countFPS;
	int m_countUpdateTime;
	int m_countDrawTime;

	Phase*			m_phase;
	static Phase*	m_nextPhase;

	static int m_scoreBlue;
	static int m_scoreRed;
	static MainImport*	 m_mainImport;
	static FbxTexImport* m_fbxTexImport;

	Renderer*	m_renderer;
	Sound*		m_sound;
	Debugproc*	m_debugproc;

	Keyboard*	 m_keyboard;
	PadXManager* m_padXManager;
	Load*		 m_load;
};

//=============================================================================
#endif

// [EOF]
