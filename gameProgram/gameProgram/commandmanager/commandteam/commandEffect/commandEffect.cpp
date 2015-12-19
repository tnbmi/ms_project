//*****************************************************************************
//
// コマンドエフェクト用ポリゴン [commandEffect.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandEffect.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CommandEffect::CommandEffect(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : Polygon2D(device, objectList, type)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
}

//=============================================================================
// デストラクタ
//=============================================================================
CommandEffect::~CommandEffect(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool CommandEffect::Create(CommandEffect** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type)
{
	CommandEffect* pointer = new CommandEffect(device, objectList, type);
	if(!pointer->Initialize(texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool CommandEffect::Initialize(LPDIRECT3DTEXTURE9 texture)
{
	//----------------------------
	// 親の初期化呼び出し
	//----------------------------
	Polygon2D::Initialize(texture);

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void CommandEffect::Finalize(void)
{
	//----------------------------
	// 親の終了呼び出し
	//----------------------------
	Polygon2D::Finalize();
}

//=============================================================================
// 更新
//=============================================================================
void CommandEffect::Update(void)
{
	//----------------------------
	// 親の更新呼び出し
	//----------------------------
	Polygon2D::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CommandEffect::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	// 加算合成
	m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//----------------------------
	// 親の描画呼び出し
	//----------------------------
	Polygon2D::Draw(vsc, psc, vp);

	// 加算合成もういらない
	m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

// EOF
