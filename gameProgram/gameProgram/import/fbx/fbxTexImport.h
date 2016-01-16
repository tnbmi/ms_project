//*****************************************************************************
//
// FbxTexインポート [fbxTexImport.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef FBXTEXIMPORT_H_
#define FBXTEXIMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FbxTexImport
{
public:
	FbxTexImport();
	~FbxTexImport(void);

	static bool Create(FbxTexImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);
	LPDIRECT3DTEXTURE9 LoadTexture( const char *texPath );
	

private:

	LPDIRECT3DDEVICE9 m_device;

	struct FBXTEXDATA
	{
		LPDIRECT3DTEXTURE9 m_tex;
		char *texPath;
	};

	static const int _buffMax = 20;
	FBXTEXDATA m_texDataArray[_buffMax];
};

//=============================================================================
#endif

// EOF