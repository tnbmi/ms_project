//*****************************************************************************
//
// インポートの基盤 [import.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "import.h"
#include "..\common\safe.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Import::Import(int textureMax)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_texture = new LPDIRECT3DTEXTURE9[textureMax];
}

//=============================================================================
// デストラクタ
//=============================================================================
Import::~Import(void)
{
	SafeDeleteArray(m_texture);
}

// EOF
