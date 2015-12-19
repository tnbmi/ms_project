//*****************************************************************************
//
// 替え玉XInputゲームパッド入力 [dummyPadX.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_DUMMY_PAD_X_H
#define _MY_DUMMY_PAD_X_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\padX\padX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;

class DummyPadX : public PadX
{
public:
	DummyPadX(void);
	~DummyPadX(void);

	static bool Create(DummyPadX** outPointer, int no);
	bool Initialize(int no);
	void Finalize(void);
	void Update(void);

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

	bool buttonPress(WORD wButton) {return (m_state.wButtons & wButton) ? true : false;}
	bool buttonTrigger(WORD wButton) {return (m_trigger.wButtons & wButton) ? true : false;}
	bool buttonRelease(WORD wButton) {return (m_release.wButtons & wButton) ? true : false;}

	bool triggerLPress(void) {return (m_state.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	bool triggerLTrigger(void) {return (m_trigger.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	bool triggerLRelease(void) {return (m_release.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}

	bool triggerRPress(void) {return (m_state.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	bool triggerRTrigger(void) {return (m_trigger.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	bool triggerRRelease(void) {return (m_release.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}

	SHORT thumbLX(void) {return (abs(m_state.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? m_state.sThumbLX : 0;}
	SHORT thumbLY(void) {return (abs(m_state.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? m_state.sThumbLY : 0;}
	SHORT thumbRX(void) {return (abs(m_state.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? m_state.sThumbRX : 0;}
	SHORT thumbRY(void) {return (abs(m_state.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? m_state.sThumbRY : 0;}
};

//=============================================================================
#endif

// [EOF]
