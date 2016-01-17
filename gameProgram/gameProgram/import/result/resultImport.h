//*****************************************************************************
//
// リザルトのインポート [resultImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_RESULT_IMPORT_H_
#define MY_RESULT_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ResultImport : public Import
{
public:
	enum TEX_TABLE
	{
		// なし
		NONE = 0,

		GROUND,
		BG,
		SKY,

		NUM,
		SYOURIRED,
		SYOURIBLUE,
		KEKKA,
		REDOUGI,
		BLUEOUGI,
		REDHIKIWAKE,
		BLUEHIKIWAKE,

		TEX_MAX
	};

	ResultImport(void);
	~ResultImport(void);

	static bool Create(ResultImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
