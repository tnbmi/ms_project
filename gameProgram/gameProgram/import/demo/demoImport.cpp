//*****************************************************************************
//
// デモのインポート [demoImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "demoImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// テクスチャ
//----------------------------
const char* _demo_texture_name[] =
{
	nullptr,

	"/ground1.png",
	"/BG1.png",
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
	"/CutinRed.png",
	"/Cutinblue.png",
	"/ougi.png",
	"/effect.jpg"
};

//=============================================================================
// コンストラクタ
//=============================================================================
DemoImport::DemoImport(void) : Import(TEX_MAX)
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
DemoImport::~DemoImport(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool DemoImport::Create(DemoImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	DemoImport* pointer = new DemoImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool DemoImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _demo_texture_name[cnt]);

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
void DemoImport::Finalize(void)
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
