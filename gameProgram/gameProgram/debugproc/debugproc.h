//*****************************************************************************
//
// デバッグ表示 [debugproc.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_DEBUGPROC_H
#define _MY_DEBUGPROC_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define STR_MAX	(2048)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc
{
public:
	Debugproc(void);
	~Debugproc(void);

	static bool Create(Debugproc** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	void PrintDebugProc(const char *fmt, ...);

	bool drawFlg(void) {return m_drawFlg;}

private:
	LPDIRECT3DDEVICE9 m_device;

	LPD3DXFONT	m_font;
	char		m_strDebug[STR_MAX];
	bool		m_drawFlg;
};

//=============================================================================
#endif

// [EOF]
