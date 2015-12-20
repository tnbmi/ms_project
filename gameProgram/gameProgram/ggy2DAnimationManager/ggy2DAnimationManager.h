//*****************************************************************************
//
// おじちゃんのアニメーション制御2D [ggy2DAnimatioManager.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef GGY2DANIMATION_MANAGER_H_
#define GGY2DANIMATION_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ObjectList;
class UpdateList;
class DrawListManager;
class Polygon2D;
class Tex2DAnimation;

class Ggy2DAnimationManager
{
public:
	Ggy2DAnimationManager( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList);
	~Ggy2DAnimationManager(void);

	static bool Create(Ggy2DAnimationManager** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList
						);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	void SetTexture( const int idx,LPDIRECT3DTEXTURE9 tex );

	void pos( const D3DXVECTOR3 &pos ){ m_pos = pos; }
	D3DXVECTOR3 pos(){ return m_pos;}

	void scl( const D3DXVECTOR3 &scl ){ m_scl = scl; }
	D3DXVECTOR3 scl(){ return m_scl;}

	void StartAnimation( const int idx,bool isRoop );
	bool isEndAnimation();

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_scl;
	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	static const int _animMax = 9;
	Tex2DAnimation **m_animArray;
	Polygon2D*		m_poly;
	int m_animIdx;
};

//=============================================================================
#endif

// EOF
