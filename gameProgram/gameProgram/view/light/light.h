//*****************************************************************************
//
// ライト設定 [light.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_LIGHT_H_
#define MY_LIGHT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Light
{
public:
	// 平行光源
	struct DIR_LIGHT
	{
		D3DXVECTOR3	dir;
		D3DXCOLOR	diffuse;
		D3DXCOLOR	ambient;
	};

	Light(void);
	virtual ~Light(void);

	static bool Create(Light** outPointer);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	DIR_LIGHT m_dirLight;
};

//=============================================================================
#endif

// EOF
