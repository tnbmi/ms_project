//*****************************************************************************
//
// タイトルのインポート [titleImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "titleImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// テクスチャ
//----------------------------
const char* _title_texture_name[] =
{
	nullptr,

	"/titleLogo.png",
	"/BG1.png",
	"/sky.jpg",
	"/oshitekaishikansei.png",
};

//=============================================================================
// コンストラクタ
//=============================================================================
TitleImport::TitleImport(void) : Import(TEX_MAX)
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
TitleImport::~TitleImport(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool TitleImport::Create(TitleImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	TitleImport* pointer = new TitleImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool TitleImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _title_texture_name[cnt]);

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
void TitleImport::Finalize(void)
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
