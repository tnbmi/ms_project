//*****************************************************************************
//
// タイトルのインポート [titleImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TITLE_IMPORT_H_
#define MY_TITLE_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TitleImport : public Import
{
public:
	enum TEX_TABLE
	{
		// なし
		NONE = 0,

		TITLE_LOGO,
		SKY,

		TEX_MAX
	};

	TitleImport(void);
	~TitleImport(void);

	static bool Create(TitleImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
