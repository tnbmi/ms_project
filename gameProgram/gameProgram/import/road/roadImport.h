//*****************************************************************************
//
// ゲームのインポート [roadImport.h]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

#ifndef ROAD_IMPORT_H_
#define ROAD_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"
#include "..\..\debugproc\debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RoadImport : public Import
{
public:
	enum TEX_TABLE
	{
		// なし
		NONE = 0,

		FADE_LEFT,
		FADE_RIGHT,

		ICON,
		TEXT,

		TEX_MAX
	};

	RoadImport(void);
	~RoadImport(void);

	static bool Create(RoadImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
