//*****************************************************************************
//
// 2Dポリゴンの基盤 [polygon2D.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_POLYGON_2D_H_
#define MY_POLYGON_2D_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Polygon2D : public ObjectBase
{
public:
	struct VERTEX
	{
		D3DXVECTOR3	position;
		D3DCOLOR	color;
		D3DXVECTOR2	uv;
	};

	Polygon2D(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type);
	virtual ~Polygon2D(void);

	static bool Create(Polygon2D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, int priority = 0, OBJECT_TYPE type = ObjectBase::TYPE_2D);
	virtual bool Initialize(LPDIRECT3DTEXTURE9 texture);
	virtual void Finalize(void);
	virtual void Update(void);
	virtual void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

	virtual D3DXMATRIX world(void) {return m_world;}

	virtual D3DXVECTOR3	pos(void) {return m_pos;}
	virtual void		pos(D3DXVECTOR3 pos) {m_pos = pos;}
	virtual void		pos(float x, float y, float z) {m_pos = D3DXVECTOR3(x,y,z);}
	virtual void		pos_x(float x) {m_pos.x = x;}
	virtual void		pos_y(float y) {m_pos.y = y;}
	virtual void		pos_z(float z) {m_pos.z = z;}

	virtual D3DXVECTOR3	rot(void) {return m_rot;}
	virtual void		rot(D3DXVECTOR3 rot) {m_rot = rot;}
	virtual void		rot(float x, float y, float z) {m_rot = D3DXVECTOR3(x,y,z);}
	virtual void		rot_x(float x) {m_rot.x = x;}
	virtual void		rot_y(float y) {m_rot.y = y;}
	virtual void		rot_z(float z) {m_rot.z = z;}

	virtual D3DXVECTOR3	scl(void) {return m_scl;}
	virtual void		scl(D3DXVECTOR3 scl) {m_scl = scl; SetVertex();}
	virtual void		scl(float x, float y, float z) {m_scl = D3DXVECTOR3(x,y,z);}
	virtual void		scl_x(float x) {m_scl.x = x;}
	virtual void		scl_y(float y) {m_scl.y = y;}
	virtual void		scl_z(float z) {m_scl.z = z;}

	virtual D3DXCOLOR	color(void) {return m_color;}
	virtual void		color(D3DXCOLOR color) {m_color = color;}
	virtual void		color(float r, float g, float b, float a) {m_color = D3DXCOLOR(r,g,b,a);}
	virtual void		color_r(float r) {m_color.r = r;}
	virtual void		color_g(float g) {m_color.g = g;}
	virtual void		color_b(float b) {m_color.b = b;}
	virtual void		color_a(float a) {m_color.a = a;}

protected:
	void SetVertex(void);

	VERTEX m_vtx[4];
};

//=============================================================================
#endif

// EOF
