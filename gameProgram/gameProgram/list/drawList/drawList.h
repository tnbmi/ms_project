//*****************************************************************************
//
// 描画管理リスト [drawList.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_DRAW_LIST_H_
#define MY_DRAW_LIST_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\list.h"

#include "..\..\shader\shader.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ObjectBase;

class DrawList : public List
{
public:
	DrawList(Shader::PATTERN shaderPatternId);
	~DrawList(void);

	static bool Create(DrawList** outPointer, Shader::PATTERN shaderPatternId);

	void Link(ObjectBase* object, int priority);
	void UnLink(ObjectBase* object);

	void AllDraw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

	static int priorityLevel(void) {return m_priorityLevel;}

private:
	static int m_priorityLevel;

	Shader::PATTERN	m_shaderPatternId;
};

//=============================================================================
#endif

// EOF
