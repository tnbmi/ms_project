//*****************************************************************************
//
// リザルトのインポート [resultImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "resultImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// テクスチャ
//----------------------------
const char* _result_texture_name[] =
{
	nullptr,

	"/ground1.png",
	"/BG1.png",
	"/sky.jpg",

	"/number.png",
	"/syouriscrollRed.png",
	"/syouriscrollBlue.png",
	"/kekkaUI.png",
	"/redougi.png",
	"/blueougi.png",
	"/redhikiwake.png",
	"/bluehikiwake.png",
};

//=============================================================================
// コンストラクタ
//=============================================================================
ResultImport::ResultImport(void) : Import(TEX_MAX)
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
ResultImport::~ResultImport(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool ResultImport::Create(ResultImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	ResultImport* pointer = new ResultImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool ResultImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _result_texture_name[cnt]);

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
void ResultImport::Finalize(void)
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
