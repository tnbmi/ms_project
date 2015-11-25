//*****************************************************************************
//
// 描画管理リスト [drawList.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "drawList.h"
#include "..\..\common\safe.h"

#include "..\..\objectBase\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int DrawList::m_priorityLevel = 5;

//=============================================================================
// コンストラクタ
//=============================================================================
DrawList::DrawList(Shader::PATTERN shaderPatternId) : List(m_priorityLevel)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	for(int cnt = 0; cnt < m_priorityLevel; ++cnt)
	{
		m_top[cnt] = nullptr;
		m_end[cnt] = nullptr;
	}

	m_shaderPatternId = shaderPatternId;
}

//=============================================================================
// デストラクタ
//=============================================================================
DrawList::~DrawList(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool DrawList::Create(DrawList** outPointer, Shader::PATTERN shaderPatternId)
{
	DrawList* pointer = new DrawList(shaderPatternId);

	*outPointer = pointer;
	return true;
}

//=============================================================================
// リンク
//=============================================================================
void DrawList::Link(ObjectBase* object, int priority)
{
	//----------------------------
	// 描画リストを設定
	//----------------------------
	object->drawList(this, m_shaderPatternId);
	object->priority(m_shaderPatternId, priority);

	//----------------------------
	// 前オブジェクトの設定
	//----------------------------
	if(m_top[priority] == nullptr)
	{
		// 先頭に設定
		m_top[priority] = object;
		object->drawPrev(m_shaderPatternId);
	}
	else
	{
		// 前オブジェクトから連結
		m_end[priority]->drawNext(m_shaderPatternId, object);
		object->drawPrev(m_shaderPatternId, m_end[priority]);
	}

	//----------------------------
	// 次オブジェクトの設定
	//----------------------------
	object->drawNext(m_shaderPatternId, nullptr);

	//----------------------------
	// 終端オブジェクトの設定
	//----------------------------
	m_end[priority] = object;
}

//=============================================================================
// リンク解除
//=============================================================================
void DrawList::UnLink(ObjectBase* object)
{
	// 存在チェック
	if(object->drawList(m_shaderPatternId) == nullptr)
		return;

	ObjectBase*	prev	 = object->drawPrev(m_shaderPatternId);
	ObjectBase*	next	 = object->drawNext(m_shaderPatternId);
	int			priority = object->priority(m_shaderPatternId);

	//----------------------------
	// 前オブジェクトの再設定
	//----------------------------
	object->drawPrev(m_shaderPatternId, nullptr);
	if(prev != nullptr)
	{
		// 前オブジェクトに次オブジェクトを連結
		prev->drawNext(m_shaderPatternId, next);
	}
	else
	{
		// 先頭オブジェクトを変更
		m_top[priority] = next;

		if(m_top[priority] != nullptr)
			// 先頭の前をnullptrに
			m_top[priority]->drawPrev(m_shaderPatternId, nullptr);
	}

	//----------------------------
	// 次オブジェクトの再設定
	//----------------------------
	object->drawNext(m_shaderPatternId, nullptr);
	if(next != nullptr)
	{
		// 次オブジェクトに前オブジェクトを連結
		next->drawPrev(m_shaderPatternId, prev);
	}
	else
	{
		// 終端オブジェクトを変更
		m_end[priority] = prev;

		if(m_end[priority] != nullptr)
			// 終端をnullptrに
			m_end[priority]->drawNext(m_shaderPatternId, nullptr);
	}

	//----------------------------
	// 描画リストを解除
	//----------------------------
	object->drawList(nullptr, m_shaderPatternId);
}

//=============================================================================
// 全オブジェクトの描画
//=============================================================================
void DrawList::AllDraw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	ObjectBase* cur = nullptr;

	for(int cnt = 0; cnt < m_priorityLevel; ++cnt)
	{
		cur = m_top[cnt];

		while(cur)
		{
			cur->Draw(vsc, psc, vp);

			cur = cur->drawNext(m_shaderPatternId);
		}
	}
}

// EOF
