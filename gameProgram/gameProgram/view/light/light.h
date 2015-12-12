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

	static bool Create(Light** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);

	void SetLight(LPD3DXCONSTANTTABLE vsc);

	void dirLightVector(D3DXVECTOR3 vector) {m_dirLight.vector = vector; D3DXVec3Normalize(&m_dirLight.vector, &m_dirLight.vector);}
	void dirLightVector(float x, float y, float z) {m_dirLight.vector = D3DXVECTOR3(x,y,z);}
	void dirLightVector_x(float x) {m_dirLight.vector.x = x;}
	void dirLightVector_y(float y) {m_dirLight.vector.y = y;}
	void dirLightVector_z(float z) {m_dirLight.vector.z = z;}

	void dirLightDiffuse(D3DXCOLOR diffuse) {m_dirLight.diffuse = diffuse;}
	void dirLightDiffuse(float r, float g, float b, float a) {m_dirLight.diffuse = D3DXCOLOR(r,g,b,a);}
	void dirLightDiffuse_r(float r) {m_dirLight.diffuse.r = r;}
	void dirLightDiffuse_g(float g) {m_dirLight.diffuse.g = g;}
	void dirLightDiffuse_b(float b) {m_dirLight.diffuse.b = b;}
	void dirLightDiffuse_a(float a) {m_dirLight.diffuse.a = a;}

	void dirLightAmbient(D3DXCOLOR ambient) {m_dirLight.ambient = ambient;}
	void dirLightAmbient(float r, float g, float b, float a) {m_dirLight.ambient = D3DXCOLOR(r,g,b,a);}
	void dirLightAmbient_r(float r) {m_dirLight.ambient.r = r;}
	void dirLightAmbient_g(float g) {m_dirLight.ambient.g = g;}
	void dirLightAmbient_b(float b) {m_dirLight.ambient.b = b;}
	void dirLightAmbient_a(float a) {m_dirLight.ambient.a = a;}

private:
	LPDIRECT3DDEVICE9 m_device;

	DIR_LIGHT m_dirLight;
};

//=============================================================================
#endif

// EOF
