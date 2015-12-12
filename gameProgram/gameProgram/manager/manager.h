//*****************************************************************************
//
// ���C���}�l�[�W���[ [manager.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MANAGER_H
#define _MY_MANAGER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Phase;
class Renderer;
class Debugproc;
class Keyboard;
class PadXManager;
class Road;

class Manager
{
public:
	Manager(void);
	virtual ~Manager(void);

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
	Debugproc*	m_debugproc;

	Keyboard*	 m_keyboard;
	PadXManager* m_padXManager;
	Road*		 m_road;
};

//=============================================================================
#endif

// [EOF]
