//*****************************************************************************
//
// シェーダー管理 [shader.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_SHADER_H_
#define MY_SHADER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Shader
{
public:
	enum PATTERN
	{
		PAT_NONE = 0,
		PAT_LIGHT,
		PAT_NONE_LIGHT,
		PAT_FBX,
		PAT_INS,
		PAT_2D,

		PATTERN_MAX
	};

	Shader(LPDIRECT3DDEVICE9 device);
	~Shader(void);

	static bool Create(Shader** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(void);
	void Finalize(void);

	void SetShader(LPD3DXCONSTANTTABLE* outVSC, LPD3DXCONSTANTTABLE* outPSC, PATTERN patternId);

	LPD3DXCONSTANTTABLE vsc(int id){return m_vsc[id];}
	LPD3DXCONSTANTTABLE psc(int id){return m_psc[id];}

private:
	LPDIRECT3DDEVICE9 m_device;

	int m_vsSize;
	LPDIRECT3DVERTEXSHADER9* m_vertexShader;	// バーテックスシェーダー
	LPD3DXCONSTANTTABLE*	 m_vsc;				// バーテックスシェーダーコンスタントテーブル

	int m_psSize;
	LPDIRECT3DPIXELSHADER9*	m_pixelShader;		// ピクセルシェーダー
	LPD3DXCONSTANTTABLE*	m_psc;				// ピクセルシェーダーコンスタントテーブル
};

//=============================================================================
#endif

// EOF
