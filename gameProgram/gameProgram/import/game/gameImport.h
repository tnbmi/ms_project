//*****************************************************************************
//
// ゲームのインポート [gameImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_GAME_IMPORT_H_
#define MY_GAME_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class GameImport : public Import
{
public:
	enum TEX_TABLE
	{
		// なし
		NONE = 0,

		TEST_0,
		TEST_1,

		COMMAND_TEX,
		BLUE_TEX,
		RED_TEX,
		COMMAND_FRAM_00,
		COMMAND_FRAM_01,
		COMMAND_FRAM_02,
		NUMBER_TEX,
		NUMBER_YELLOW,
		NUMBER_WHITE,

		TEX_MAX
	};

	GameImport(void);
	~GameImport(void);

	static bool Create(GameImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
