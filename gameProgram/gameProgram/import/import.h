//*****************************************************************************
//
// インポートの基盤 [import.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_IMPORT_H_
#define MY_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define TEXTURE_PATH ("./resources/texture")

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Import
{
public:
	Import(int textureMax);
	virtual ~Import(void);

	virtual bool Initialize(LPDIRECT3DDEVICE9 device) = 0;
	virtual void Finalize(void) = 0;

	virtual LPDIRECT3DTEXTURE9 texture(int id) {return m_texture[id];}

protected:
	LPDIRECT3DTEXTURE9* m_texture;
};

//=============================================================================
#endif

// EOF
