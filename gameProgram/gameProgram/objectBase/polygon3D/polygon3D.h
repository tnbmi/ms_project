//*****************************************************************************
//
// 3Dポリゴンの基盤 [polygon3D.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_POLYGON_3D_H_
#define MY_POLYGON_3D_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Polygon3D : public ObjectBase
{
public:
	struct VERTEX
	{
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
		D3DXVECTOR2	uv;
	};

	Polygon3D(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type);
	virtual ~Polygon3D(void);

	static bool Create(Polygon3D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,
					LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type = ObjectBase::TYPE_3D);
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
	virtual void		scl(D3DXVECTOR3 scl) {m_scl = scl; SetVertexScl();}
	virtual void		scl(float x, float y, float z) {m_scl = D3DXVECTOR3(x,y,z); SetVertexScl();}
	virtual void		scl_x(float x) {m_scl.x = x; SetVertexScl();}
	virtual void		scl_y(float y) {m_scl.y = y; SetVertexScl();}
	virtual void		scl_z(float z) {m_scl.z = z; SetVertexScl();}

	virtual D3DXCOLOR	color(void) {return m_color;}
	virtual void		color(D3DXCOLOR color) {m_color = color; SetVertexColor();}
	virtual void		color(float r, float g, float b, float a) {m_color = D3DXCOLOR(r,g,b,a); SetVertexColor();}
	virtual void		color_r(float r) {m_color.r = r; SetVertexColor();}
	virtual void		color_g(float g) {m_color.g = g; SetVertexColor();}
	virtual void		color_b(float b) {m_color.b = b; SetVertexColor();}
	virtual void		color_a(float a) {m_color.a = a; SetVertexColor();}

	virtual void texcoord(int num, D3DXVECTOR2 uv) {m_vtx[num].uv = uv;}
	virtual void texcoord(int num, float u, float v) {m_vtx[num].uv.x = u; m_vtx[num].uv.y = v;}
	virtual void texcoord_u(int num, float u) {m_vtx[num].uv.x = u;}

	virtual void texture(LPDIRECT3DTEXTURE9 texture){m_texture = texture;}
	virtual void norTexture(LPDIRECT3DTEXTURE9 norTexture){m_norTexture = norTexture;}

protected:
	void SetVertex(void);
	void SetVertexScl(void);
	void SetVertexColor(void);

	VERTEX m_vtx[4];

	LPDIRECT3DTEXTURE9 m_norTexture;
};

//=============================================================================
#endif

// EOF
