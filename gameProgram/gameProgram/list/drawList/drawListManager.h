//*****************************************************************************
//
// 描画管理リストマネージャー [drawList.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_DRAW_LIST_MANAGER_H_
#define MY_DRAW_LIST_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

#include "..\..\shader\shader.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DrawList;
class ObjectBase;

class DrawListManager
{
public:
	DrawListManager(LPDIRECT3DDEVICE9 device);
	~DrawListManager(void);

	static bool Create(DrawListManager** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(void);
	void Finalize(void);

	void Link(ObjectBase* object, int priority, Shader::PATTERN shaderPatternId);
	void UnLink(ObjectBase* object, Shader::PATTERN shaderPatternId);

	void AllDraw(D3DXMATRIX vp);

private:
	LPDIRECT3DDEVICE9 m_device;

	Shader*		m_shader;
	DrawList*	m_drawList[Shader::PATTERN_MAX];

	//LPDIRECT3DVERTEXDECLARATION9 m_decl2D;
	LPDIRECT3DVERTEXDECLARATION9 m_decl3D;
};

//=============================================================================
#endif

// EOF
