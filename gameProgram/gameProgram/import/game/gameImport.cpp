//*****************************************************************************
//
// ゲームのインポート [gameImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "gameImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// テクスチャ
//----------------------------
const char* _game_texture_name[] =
{
	nullptr,

	"/stones.bmp",
	"/stones_normal.bmp",
	"/sky.jpg",

	"/COMMAND_TEX.png",
	"/blue.png",
	"/red.png",
	"/commandfram00.png",
	"/commandfram01.png",
	"/number.png",
	"/num_yellow.png",
	"/num_white.png",
	"/GameUI.png",
	"/B�@.png",
	"/B�A.png",
	"/B�B.png",
	"/B�C.png",
	"/B�D.png",
	"/B�E.png",
	"/B�F.png",
	"/B�G.png",
	"/B�H.png",
	"/R�@.png",
	"/R�A.png",
	"/R�B.png",
	"/R�C.png",
	"/R�D.png",
	"/R�E.png",
	"/R�F.png",
	"/R�G.png",
	"/R�H.png",
};

//=============================================================================
// コンストラクタ
//=============================================================================
GameImport::GameImport(void) : Import(TEX_MAX)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	// テクスチャ
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_texture[cnt] = nullptr;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
GameImport::~GameImport(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool GameImport::Create(GameImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	GameImport* pointer = new GameImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool GameImport::Initialize(LPDIRECT3DDEVICE9 device)
{
	HRESULT	hr;
	char	str[512];

	//----------------------------
	// テクスチャ
	//----------------------------
	for(int cnt = 1; cnt < TEX_MAX; ++cnt)
	{
		// パス連結
		memset(str, 0, sizeof(str));
		strcpy_s(str, TEXTURE_PATH);
		strcat_s(str, sizeof(str), _game_texture_name[cnt]);

		// テクスチャ読み込み
		hr = D3DXCreateTextureFromFile(device, str, &m_texture[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, "テクスチャ無し", "D3DXCreateTextureFromFile", MB_OK);
		}
	}

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void GameImport::Finalize(void)
{
	//----------------------------
	// テクスチャ
	//----------------------------
	for(int cnt = 1; cnt < TEX_MAX; cnt++)
	{
		// テクスチャの開放
		SafeRelease(m_texture[cnt]);
	}
}

// EOF
