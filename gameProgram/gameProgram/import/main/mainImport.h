//*****************************************************************************
//
// メインのインポート [mainImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_MAIN_IMPORT_H_
#define MY_MAIN_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class MainImport : public Import
{
public:
	enum TEX_TABLE
	{
		// なし
		NONE = 0,

		STONES,
		STONES_NOR,
		SKY,

		COMMAND_TEX,
		BLUE_TEX,
		RED_TEX,
		COMMAND_FRAM_00,
		COMMAND_FRAM_01,
		NUMBER_TEX,
		NUMBER_YELLOW,
		NUMBER_WHITE,
		MAIN_UI,
		GGYBLUE_WAIT,
		GGYBLUE_WIN,
		GGYBLUE_LOSE,
		GGYBLUE_UP,
		GGYBLUE_RIGHT,
		GGYBLUE_DOWN,
		GGYBLUE_LEFT,
		GGYBLUE_P,
		GGYBLUE_POSE,
		GGYRED_WAIT,
		GGYRED_WIN,
		GGYRED_LOSE,
		GGYRED_UP,
		GGYRED_RIGHT,
		GGYRED_DOWN,
		GGYRED_LEFT,
		GGYRED_P,
		GGYRED_POSE,
		CUTINRED,
		CUTINBLUE,

		TEX_MAX
	};

	MainImport(void);
	~MainImport(void);

	static bool Create(MainImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
