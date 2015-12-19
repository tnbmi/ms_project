//*****************************************************************************
//
// XInputゲームパッド入力 [padX.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_PAD_X_H
#define _MY_PAD_X_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define BUTTON_NUM	(14)	// ボタンの数
#define TRIGGER_NUM	(2)		// トリガーの数

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;

class PadX
{
public:
	PadX(void);
	~PadX(void);

	static bool Create(PadX** outPointer, int no);
	bool Initialize(int no);
	void Finalize(void);
	void Update(void);

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

	bool conected(void) {return m_connected;}

	virtual bool buttonPress(WORD wButton) {return (m_state.wButtons & wButton) ? true : false;}
	virtual bool buttonTrigger(WORD wButton) {return (m_trigger.wButtons & wButton) ? true : false;}
	virtual bool buttonRelease(WORD wButton) {return (m_release.wButtons & wButton) ? true : false;}

	virtual bool triggerLPress(void) {return (m_state.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	virtual bool triggerLTrigger(void) {return (m_trigger.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	virtual bool triggerLRelease(void) {return (m_release.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}

	virtual bool triggerRPress(void) {return (m_state.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	virtual bool triggerRTrigger(void) {return (m_trigger.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	virtual bool triggerRRelease(void) {return (m_release.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}

	virtual SHORT thumbLX(void) {return (abs(m_state.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? m_state.sThumbLX : 0;}
	virtual SHORT thumbLY(void) {return (abs(m_state.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? m_state.sThumbLY : 0;}
	virtual SHORT thumbRX(void) {return (abs(m_state.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? m_state.sThumbRX : 0;}
	virtual SHORT thumbRY(void) {return (abs(m_state.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? m_state.sThumbRY : 0;}

private:
	bool m_connected;

protected:
	Debugproc* m_debugproc;
	int	 m_no;

	XINPUT_GAMEPAD m_state;
	XINPUT_GAMEPAD m_trigger;
	XINPUT_GAMEPAD m_release;
	XINPUT_GAMEPAD m_repeat;

	unsigned int m_cntRepeatBtn[BUTTON_NUM];
	unsigned int m_cntRepeatTrigger[TRIGGER_NUM];
};

//=============================================================================
#endif

// [EOF]
