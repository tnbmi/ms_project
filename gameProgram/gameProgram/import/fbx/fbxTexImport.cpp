//*****************************************************************************
//
// FbxTexインポートの基盤 [fbxTexImport.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "fbxTexImport.h"
#include "..\..\common\safe.h"
#include <string.h>

//=============================================================================
// コンストラクタ
//=============================================================================
FbxTexImport::FbxTexImport()
{
	//----------------------------
	// メンバー初期化
	//----------------------------
}

//=============================================================================
// デストラクタ
//=============================================================================
FbxTexImport::~FbxTexImport(void)
{

}

//=============================================================================
//初期化
//=============================================================================

bool FbxTexImport::Initialize( LPDIRECT3DDEVICE9 device )
{
	m_device = device;

	for( int i = 0 ; i < _buffMax ; i++ )
	{
		m_texDataArray[i].m_tex = nullptr;
		m_texDataArray[i].texPath = nullptr;
	}


	return true;
}

//=============================================================================
//初期化
//=============================================================================

void FbxTexImport::Finalize()
{
	for( int i = 0 ; i < _buffMax ; i++ )
	{
		SafeRelease( m_texDataArray[i].m_tex );
		SafeDeleteArray( m_texDataArray[i].texPath );
	}
}

//=============================================================================
//
//=============================================================================

bool FbxTexImport::Create(FbxTexImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	FbxTexImport *imp = new FbxTexImport();
	if( !imp->Initialize(device) )
	return false;

	*outPointer = imp;

	return true;
}

//=============================================================================
//
//=============================================================================

LPDIRECT3DTEXTURE9 FbxTexImport::LoadTexture( const char *texPath )
{
	for( int i = 0 ; i < _buffMax ; i++ )
	{
		if( m_texDataArray[i].texPath != nullptr )
		{
			if( strcmp( m_texDataArray[i].texPath,texPath) == 0 )
			{
				return m_texDataArray[i].m_tex;
			}
		}
		else
		{
			if( FAILED( D3DXCreateTextureFromFile(m_device,texPath,&m_texDataArray[i].m_tex) ) )
			{
				return NULL;
			}

			int len = strlen( texPath )+1;
			m_texDataArray[i].texPath = new char[ len ];
			strcpy( m_texDataArray[i].texPath,texPath );

			return m_texDataArray[i].m_tex;
		}
	}

	return NULL;
}

// EOF