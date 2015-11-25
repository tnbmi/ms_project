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
		D3DXVECTOR3	vector;
		D3DXCOLOR	diffuse;
		D3DXCOLOR	ambient;
	};

	Light(LPDIRECT3DDEVICE9 device);
	~Light(void);

	static bool Create(Light** outPointer, LPDIRECT3DDEVICE9 device, LPD3DXCONSTANTTABLE vsc);
	bool Initialize(LPD3DXCONSTANTTABLE vsc);
	void Finalize(void);
	void Update(void);

	void SetLight(void);

	void dirLightVector(D3DXVECTOR3 vector) {m_dirLight.vector = vector;}
	void dirLightDiffuse(D3DXCOLOR diffuse) {m_dirLight.diffuse = diffuse;}
	void dirLightAmbient(D3DXCOLOR ambient) {m_dirLight.ambient = ambient;}

private:
	LPDIRECT3DDEVICE9	m_device;
	LPD3DXCONSTANTTABLE m_vsc;

	DIR_LIGHT m_dirLight;
};

//=============================================================================
#endif

// EOF
