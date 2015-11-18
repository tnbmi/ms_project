//*****************************************************************************
//
// シェーダー管理 [shader.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "shader.h"
#include "..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// シェーダーデータ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// シェーダーファイルパス
const char* _shader_path = "../resources/shader";

//----------------------------
// バーテックスシェーダ
//----------------------------
// ナンバリング
enum VS_TABLE
{
	VS_NONE	 = -1,
	VS_2D	 = 0,
	VS_LIGHT,
	VS_FBX,
	VS_INS,
};

// ファイルパス
const char* _vsName[] =
{
	{"/twoDimensionsVS.hlsl"},
	{"/lightVS.hlsl"},
	{"/fbxVS.hlsl"},
	{"/instancingVS.hlsl"}
};

//----------------------------
// ピクセルシェーダ
//----------------------------
// ナンバリング
enum PS_TABLE
{
	PS_NONE = -1,
	PS_2D = 0,
	PS_BASIC,
	PS_FBX,
	PS_INS
};

// ファイルパス
const char* _psName[] =
{
	{"/twoDimensionsPS.hlsl"},
	{"/basicPS.hlsl"},
	{"/fbxPS.hlsl"},
	{"/instancingPS.hlsl"}
};

//----------------------------
// パターンのセット
//----------------------------
// IDセット
struct SHADER_SET
{
	VS_TABLE vscID;
	PS_TABLE pscID;
};

// パターンセット内容
const SHADER_SET _shader_set[Shader::PATTERN_MAX] =
{
	{VS_NONE,	PS_NONE},		// NONE
	{VS_2D,		PS_2D},			// 2D
	{VS_LIGHT,	PS_BASIC},		// LIGHT
	{VS_FBX,	PS_FBX},		// FBX
	{VS_INS,	PS_INS}
};

//=============================================================================
// コンストラクタ
//=============================================================================
Shader::Shader(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_device = device;

	m_vsSize = 0;
	m_vertexShader		= nullptr;
	m_vsConstantTable	= nullptr;

	m_psSize = 0;
	m_pixelShader		= nullptr;
	m_psConstantTable	= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Shader::~Shader(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Shader::Create(Shader** outPointer, LPDIRECT3DDEVICE9 device)
{
	Shader* pointer = new Shader(device);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Shader::Initialize(void)
{
	HRESULT		 hr;
	LPD3DXBUFFER err	= 0;
	LPD3DXBUFFER code	= 0;
	char		 str[512];

	//------------------------------
	// バーテックスシェーダー
	//------------------------------
	m_vsSize			= sizeof(_vsName) / sizeof(char*);
	m_vertexShader		= new LPDIRECT3DVERTEXSHADER9[m_vsSize];
	m_vsConstantTable	= new LPD3DXCONSTANTTABLE[m_vsSize];

	for(int cnt = 0; cnt < m_vsSize; ++cnt)
	{
		// パス連結
		memset(str, 0, sizeof(str));
		strcpy_s(str, _shader_path);
		strcat_s(str, sizeof(str), _vsName[cnt]);

		// シェーダーファイル読み込み
		hr = D3DXCompileShaderFromFile(str, NULL, NULL,
										"VS", "vs_3_0", 0, &code, &err, &m_vsConstantTable[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
			err->Release();
			return false;
		}

		// 設定
		hr = m_device->CreateVertexShader((DWORD*)code->GetBufferPointer(), &m_vertexShader[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, "FAILED", "CreateVertexShader", MB_OK);
			return false;
		}
	}

	//------------------------------
	// ピクセルシェーダー
	//------------------------------
	m_psSize			= sizeof(_psName) / sizeof(char*);
	m_pixelShader		= new LPDIRECT3DPIXELSHADER9[m_psSize];
	m_psConstantTable	= new LPD3DXCONSTANTTABLE[m_psSize];

	for(int cnt = 0; cnt < m_psSize; ++cnt)
	{
		// パス連結
		memset(str, 0, sizeof(str));
		strcpy_s(str, _shader_path);
		strcat_s(str, sizeof(str), _psName[cnt]);

		// シェーダーファイル読み込み
		hr = D3DXCompileShaderFromFile(str, NULL, NULL,
										"PS", "ps_3_0", 0, &code, &err,&m_psConstantTable[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
			err->Release();
			return false;
		}

		// 設定
		hr = m_device->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_pixelShader[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, "FAILED", "CreatePixelShader", MB_OK);
			return false;
		}
	}

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Shader::Finalize(void)
{
	// バーテックス
	for(int cnt = 0; cnt < m_vsSize; ++cnt)
	{
		SafeRelease(m_vertexShader[cnt]);
		SafeRelease(m_vsConstantTable[cnt]);
	}
	SafeDeleteArray(m_vertexShader);
	SafeDeleteArray(m_vsConstantTable);

	// ピクセル
	for(int cnt = 0; cnt < m_psSize; ++cnt)
	{
		SafeRelease(m_pixelShader[cnt]);
		SafeRelease(m_psConstantTable[cnt]);
	}
	SafeDeleteArray(m_pixelShader);
	SafeDeleteArray(m_psConstantTable);
}

//=============================================================================
// シェーダー設定
//=============================================================================
void Shader::SetShader(LPD3DXCONSTANTTABLE* outVSC, LPD3DXCONSTANTTABLE* outPSC, PATTERN patternId)
{
	//----------------------------
	// バーテックスシェーダー設定
	//----------------------------
	VS_TABLE vsID = _shader_set[patternId].vscID;

	if(vsID >= 0)
	{
		// バーテックスシェーダー
		m_device->SetVertexShader(m_vertexShader[vsID]);

		// バーテックスシェーダーテーブル
		*outVSC = m_vsConstantTable[vsID];
	}
	else
	{
		// バーテックスシェーダー
		m_device->SetVertexShader(nullptr);

		// バーテックスシェーダーテーブル
		*outVSC = nullptr;
	}

	//----------------------------
	// ピクセルシェーダー設定
	//----------------------------
	PS_TABLE psID = _shader_set[patternId].pscID;

	if(psID >= 0)
	{
		// ピクセルシェーダー
		m_device->SetPixelShader(m_pixelShader[psID]);

		// ピクセルシェーダーテーブル
		*outPSC = m_psConstantTable[psID];
	}
	else
	{
		// ピクセルシェーダー
		m_device->SetPixelShader(nullptr);

		// ピクセルシェーダーテーブル
		*outPSC = nullptr;
	}
}

// EOF
