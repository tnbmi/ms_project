//*****************************************************************************
//
// フェーズ基盤 [phase.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_PHASE_H_
#define MY_PHASE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;
class Keyboard;
class PadXManager;
class Fade;
class Sound;

class Phase
{
public:
	Phase(LPDIRECT3DDEVICE9 device);
	virtual ~Phase(void);

	virtual bool Initialize(void)	= 0;
	virtual void Finalize(void)		= 0;
	virtual void Update(void)		= 0;
	virtual void Draw(void)			= 0;

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}
	void keyboard(Keyboard* keyboard) {m_keyboard = keyboard;}
	void padXManager(PadXManager* padXManager) {m_padXManager = padXManager;}
	void sound(Sound* sound) {m_sound = sound;}

protected:
	LPDIRECT3DDEVICE9	m_device;	// Deviceオブジェクト(デバイスへのポインタ)
	Debugproc*			m_debugproc;

	Keyboard*	 m_keyboard;
	PadXManager* m_padXManager;

	Fade*	m_fade;
	Sound*	m_sound;
};

//=============================================================================
#endif

// EOF
