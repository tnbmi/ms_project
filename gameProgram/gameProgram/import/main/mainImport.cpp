//*****************************************************************************
//
// メインのインポート [mainImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "mainImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// テクスチャ
//----------------------------
const char* _main_texture_name[] =
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
	"/MainUI.png",
	"/B①.png",
	"/B②.png",
	"/B③.png",
	"/B④.png",
	"/B⑤.png",
	"/B⑥.png",
	"/B⑦.png",
	"/B⑧.png",
	"/B⑨.png",
	"/R①.png",
	"/R②.png",
	"/R③.png",
	"/R④.png",
	"/R⑤.png",
	"/R⑥.png",
	"/R⑦.png",
	"/R⑧.png",
	"/R⑨.png",
	"/CutinRed.png",
	"/Cutinblue.png",
};

//=============================================================================
// コンストラクタ
//=============================================================================
MainImport::MainImport(void) : Import(TEX_MAX)
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
MainImport::~MainImport(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool MainImport::Create(MainImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	MainImport* pointer = new MainImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool MainImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _main_texture_name[cnt]);

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
void MainImport::Finalize(void)
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
