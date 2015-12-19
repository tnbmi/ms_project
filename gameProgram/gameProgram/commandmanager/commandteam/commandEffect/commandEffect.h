//*****************************************************************************
//
// コマンドエフェクト用ポリゴン [commandEffect.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_COMMAND_EFFECT_H_
#define MY_COMMAND_EFFECT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\main\main.h"
#include "..\..\..\objectBase\polygon2D\polygon2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CommandEffect : public Polygon2D
{
public:
	CommandEffect(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type);
	~CommandEffect(void);

	static bool Create(CommandEffect** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type = ObjectBase::TYPE_2D);
	bool Initialize(LPDIRECT3DTEXTURE9 texture);
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

private:
};

//=============================================================================
#endif

// EOF
