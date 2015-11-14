//*****************************************************************************
//
// 描画機構 [renderer.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_RENDERER_H
#define _MY_RENDERER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;
class Phase;

class Renderer
{
public:
	Renderer(void);
	virtual ~Renderer(void);

	static bool Create(Renderer** outPointer, HWND hWnd, bool windowFlg);
	bool Initialize(HWND hWnd, bool windowFlg);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 device(void) {return m_device;}

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}
	void phase(Phase* phase) {m_phase = phase;}

private:
	LPDIRECT3DDEVICE9	m_device;
	Debugproc*			m_debugproc;
	Phase*				m_phase;
};

//=============================================================================
#endif

// [EOF]
