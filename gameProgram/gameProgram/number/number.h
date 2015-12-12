//*****************************************************************************
//
// 数字用ポリゴン [number.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_NUMBER_H_
#define MY_NUMBER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"
#include "..\objectBase\polygon2D\polygon2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Number : public Polygon2D
{
public:
	Number(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type);
	~Number(void);

	static bool Create(Number** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type = ObjectBase::TYPE_2D);
	bool Initialize(LPDIRECT3DTEXTURE9 texture);
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

	void SetNumber(int num);

	D3DXMATRIX world(void) {return m_world;}

	D3DXVECTOR3	pos(void) {return m_pos;}
	void		pos(D3DXVECTOR3 pos) {m_pos = pos;}
	void		pos(float x, float y, float z) {m_pos = D3DXVECTOR3(x,y,z);}
	void		pos_x(float x) {m_pos.x = x;}
	void		pos_y(float y) {m_pos.y = y;}
	void		pos_z(float z) {m_pos.z = z;}

	D3DXVECTOR3	rot(void) {return m_rot;}
	void		rot(D3DXVECTOR3 rot) {m_rot = rot;}
	void		rot(float x, float y, float z) {m_rot = D3DXVECTOR3(x,y,z);}
	void		rot_x(float x) {m_rot.x = x;}
	void		rot_y(float y) {m_rot.y = y;}
	void		rot_z(float z) {m_rot.z = z;}

	D3DXVECTOR3	scl(void) {return m_scl;}
	void		scl(D3DXVECTOR3 scl) {m_scl = scl; SetVertex();}
	void		scl(float x, float y, float z) {m_scl = D3DXVECTOR3(x,y,z);}
	void		scl_x(float x) {m_scl.x = x;}
	void		scl_y(float y) {m_scl.y = y;}
	void		scl_z(float z) {m_scl.z = z;}

	D3DXCOLOR	color(void) {return m_color;}
	void		color(D3DXCOLOR color) {m_color = color;}
	void		color(float r, float g, float b, float a) {m_color = D3DXCOLOR(r,g,b,a);}
	void		color_r(float r) {m_color.r = r;SetVertex();}
	void		color_g(float g) {m_color.g = g;SetVertex();}
	void		color_b(float b) {m_color.b = b;SetVertex();}
	void		color_a(float a) {m_color.a = a;SetVertex();}

	void	texcoord(int num, D3DXVECTOR2 uv) {m_vtx[num].uv = uv;}
	void	texcoord(int num, float u, float v) {m_vtx[num].uv.x = u; m_vtx[num].uv.y = v;}
	void	texcoord_u(int num, float u) {m_vtx[num].uv.x = u;}

	void	texture(LPDIRECT3DTEXTURE9 texture){m_texture = texture;}

private:
};

//=============================================================================
#endif

// EOF
