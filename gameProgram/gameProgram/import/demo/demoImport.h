//*****************************************************************************
//
// デモのインポート [demoImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_DEMO_IMPORT_H_
#define MY_DEMO_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DemoImport : public Import
{
public:
	enum TEX_TABLE
	{
		// なし
		NONE = 0,

		TEST_0,

		TEX_MAX
	};

	DemoImport(void);
	~DemoImport(void);

	static bool Create(DemoImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
