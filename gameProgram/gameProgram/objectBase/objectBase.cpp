//*****************************************************************************
//
// オブジェクトの基盤 [objectBase.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "objectBase.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawList.h"

//=============================================================================
// コンストラクタ
//=============================================================================
ObjectBase::ObjectBase(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	// protected
	m_device = device;

	D3DXMatrixIdentity(&m_world);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_texture = nullptr;

	//private
	m_type		 = type;
	m_deleteFlg	 = false;

	// ポインター
	m_objectList = objectList;
	m_objectPrev = nullptr;
	m_objectNext = nullptr;

	m_updateList = nullptr;
	m_updatePrev = nullptr;
	m_updateNext = nullptr;

	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
	{
		m_drawList[cnt] = nullptr;
		m_drawPrev[cnt] = nullptr;
		m_drawNext[cnt] = nullptr;
	}

	//----------------------------
	// オブジェクトリストへリンク
	//----------------------------
	m_objectList->Link(this);
}

//=============================================================================
// デストラクタ
//=============================================================================
ObjectBase::~ObjectBase(void)
{
	// 描画リスト除外
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
		if(m_drawList[cnt] != nullptr)
			m_drawList[cnt]->UnLink(this);

	// 更新リスト除外
	if(m_updateList)
		m_updateList->UnLink(this);

	// オブジェクトリスト除外
	m_objectList->UnLink(this);
}

// EOF
