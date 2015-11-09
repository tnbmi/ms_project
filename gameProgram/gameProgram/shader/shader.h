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
		PAT_2D,
		PAT_LIGHT,

		PATTERN_MAX
	};

	Shader(LPDIRECT3DDEVICE9 device);
	~Shader(void);

	static bool Create(Shader** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(void);
	void Finalize(void);

	void SetShader(LPD3DXCONSTANTTABLE* outVSC, LPD3DXCONSTANTTABLE* outPSC, PATTERN patternId);

private:
	LPDIRECT3DDEVICE9 m_device;

	int m_vsSize;
	LPDIRECT3DVERTEXSHADER9* m_vertexShader;	// バーテックスシェーダー
	LPD3DXCONSTANTTABLE*	 m_vsConstantTable;	// バーテックスシェーダーコンスタントテーブル

	int m_psSize;
	LPDIRECT3DPIXELSHADER9*	m_pixelShader;		// ピクセルシェーダー
	LPD3DXCONSTANTTABLE*	m_psConstantTable;	// ピクセルシェーダーコンスタントテーブル
};

//=============================================================================
#endif

// EOF
