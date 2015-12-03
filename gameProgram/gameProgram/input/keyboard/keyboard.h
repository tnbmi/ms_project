//*****************************************************************************
//
// キーボード入力 [keyboard.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_KEYBOARD_H
#define _MY_KEYBOARD_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define KEYBOARD_MAX (256)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Keyboard
{
public:
	Keyboard(void);
	~Keyboard(void);

	static bool Create(Keyboard** outPointer, HINSTANCE hInstance, HWND hWnd);
	bool Initialize(HINSTANCE hInstance, HWND hWnd);
	void Finalize(void);
	void Update(void);

	bool press(int key) {return (m_state[key] & 0x80) ? true : false;}
	bool trigger(int key) {return (m_trigger[key] & 0x80) ? true : false;}
	bool release(int key) {return (m_release[key] & 0x80) ? true : false;}
	bool repeat(int key) {return (m_repeat[key] & 0x80) ? true : false;}

private:
	LPDIRECTINPUT8		 m_input;		// DirectInputオブジェへのポインタ
	LPDIRECTINPUTDEVICE8 m_inputDevice;	// 入力処理のデバイスへのポインタ

	BYTE m_state[KEYBOARD_MAX];
	BYTE m_trigger[KEYBOARD_MAX];
	BYTE m_release[KEYBOARD_MAX];
	BYTE m_repeat[KEYBOARD_MAX];

	unsigned int m_cntRepeat[KEYBOARD_MAX];
};

//=============================================================================
#endif

// [EOF]
