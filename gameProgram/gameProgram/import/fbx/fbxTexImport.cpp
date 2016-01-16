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

const char *_fbxTexName[]=
{
	"/Ground_Texture_PNG.png",
	"/Kuruma_Texture_PNG.png",
	"/Nebuta_Blue_Texture.png",
	"/Nebuta_Red_Texture.png",
	"/Blue_Model.png",
	"/Red_Model.png",
	"/Title.png",
	"/Title_Ground_UV.png",
	"/Ground_Texture.tga",
	"/Mon_Texture.png",
	"/Stone_Texture.png",
	"/Syouridai_Texture.tga",
	"/Tree_Texture.png",
};

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

	int size = sizeof( _fbxTexName ) / sizeof( _fbxTexName[0]);

	char path[256] = "./resources/texture";

	for( int i = 0 ; i < size ; i++ )
	{
		char p[512];

		sprintf_s( p,"%s%s",path,_fbxTexName[i] );
		LoadTexture( p );
	
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
				return nullptr;
			}

			int len = strlen( texPath )+1;
			m_texDataArray[i].texPath = new char[ len ];
			strcpy_s( m_texDataArray[i].texPath,len,texPath );

			return m_texDataArray[i].m_tex;
		}
	}

	return NULL;
}

// EOF