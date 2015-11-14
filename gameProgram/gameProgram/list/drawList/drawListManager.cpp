//*****************************************************************************
//
// 描画管理リストマネージャー [drawList.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "drawListManager.h"
#include "..\..\common\safe.h"

#include "drawList.h"

//=============================================================================
// コンストラクタ
//=============================================================================
DrawListManager::DrawListManager(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_device = device;
	m_shader = nullptr;

	// リスト
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
	{
		m_drawList[cnt] = nullptr;
	}

	// 2D頂点宣言
	HRESULT hr;
	D3DVERTEXELEMENT9 velement2D[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},		// 座標
		//{0, 12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},		// 色
		//{0, 16, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},		// テクスチャ
		D3DDECL_END(),
	};

	hr = m_device->CreateVertexDeclaration(&velement2D[0], &m_decl2D);
	if(FAILED(hr))
	{
		MessageBox(NULL, "2D頂点宣言失敗", "CreateVertexDeclaration", MB_OK);
	}

	// 3D頂点宣言
	D3DVERTEXELEMENT9 velement3D[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},		// 座標
		{0, 12, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},		// 法線
		{0, 24, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},		// 色
		{0, 28, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},		// テクスチャ
		D3DDECL_END(),
	};

	hr = m_device->CreateVertexDeclaration(&velement3D[0], &m_decl3D);
	if(FAILED(hr))
	{
		MessageBox(NULL, "3D頂点宣言失敗", "CreateVertexDeclaration", MB_OK);
	}

}

//=============================================================================
// デストラクタ
//=============================================================================
DrawListManager::~DrawListManager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool DrawListManager::Create(DrawListManager** outPointer, LPDIRECT3DDEVICE9 device)
{
	DrawListManager* pointer = new DrawListManager(device);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool DrawListManager::Initialize(void)
{
	//----------------------------
	// シェーダー初期化
	//----------------------------
	if(!Shader::Create(&m_shader, m_device))
		return false;

	//----------------------------
	// 描画管理リスト初期化
	//----------------------------
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
		if(!DrawList::Create(&m_drawList[cnt], (Shader::PATTERN)cnt))
			return false;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void DrawListManager::Finalize(void)
{
	// シェーダー
	SafeFinalizeDelete(m_shader);

	// 描画リスト達
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
	{
		SafeDelete(m_drawList[cnt]);
	}
}

//=============================================================================
// リンク
//=============================================================================
void DrawListManager::Link(ObjectBase* object, int priority, Shader::PATTERN shaderPatternId)
{
	//----------------------------
	// 描画リストにリンク
	//----------------------------
	m_drawList[shaderPatternId]->Link(object, priority);
}

//=============================================================================
// リンク解除
//=============================================================================
void DrawListManager::UnLink(ObjectBase* object, Shader::PATTERN shaderPatternId)
{
	//----------------------------
	// 描画リストのリンクを解除
	//----------------------------
	m_drawList[shaderPatternId]->UnLink(object);
}

//=============================================================================
// 全オブジェクトの描画
//=============================================================================
void DrawListManager::AllDraw(D3DXMATRIX vp)
{
	ObjectBase*			 cur = nullptr;
	LPD3DXCONSTANTTABLE	 vsc;
	LPD3DXCONSTANTTABLE	 psc;

	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
	{
		if(cnt != Shader::PAT_2D)
			// 3D頂点設定
			m_device->SetVertexDeclaration(m_decl3D);
		else
		{
			// 2D頂点設定
			m_device->SetVertexDeclaration(m_decl2D);
		}

		// シェーダー設定
		m_shader->SetShader(&vsc, &psc, (Shader::PATTERN)cnt);

		if(cnt == Shader::PAT_2D)
		{
			// 2D用プロジェクション
			/*D3DXMATRIX	proj2D(2/SCREEN_WIDTH,	0.0f,			 0.0f, 0.0f,
								0.0f,			2/SCREEN_HEIGHT, 0.0f, 0.0f,
								0.0f,			0.0f,			 1.0f, 0.0f,
								0.0f,			0.0f,			 0.0f, 1.0f);*/
			D3DXMATRIX	proj2D(SCREEN_HEIGHT/SCREEN_WIDTH,	0.0f,			 0.0f, 0.0f,
								0.0f,			1.0f, 0.0f, 0.0f,
								0.0f,			0.0f,			 1.0f, 0.0f,
								0.0f,			0.0f,			 0.0f, 1.0f);
			vsc->SetMatrix(m_device, "gProj", &proj2D);
		}

		// 描画
		m_drawList[cnt]->AllDraw(vsc, psc, vp);
	}
}

// EOF
