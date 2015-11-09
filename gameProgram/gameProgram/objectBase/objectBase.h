//*****************************************************************************
//
// オブジェクトの基盤 [objectBase.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_OBJECT_BASE_H_
#define MY_OBJECT_BASE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

#include "..\shader\shader.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ObjectList;
class UpdateList;
class DrawList;

class ObjectBase
{
public:
	enum OBJECT_TYPE
	{
		TYPE_NONE = 0,
		TYPE_2D
	};

	ObjectBase(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority = 0, OBJECT_TYPE type = TYPE_NONE);
	virtual ~ObjectBase(void);

	virtual bool Initialize(void) {return true;}
	virtual void Finalize(void)		 = 0;
	virtual void Update(void)		 = 0;
	virtual void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp) = 0;

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
	virtual void		scl(D3DXVECTOR3 scl) {m_scl = scl;}
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

	int	 priority(void) {return m_priority;}
	void priority(int priority) {m_priority = priority;}

	OBJECT_TYPE type(void) {return m_type;}
	void		type(OBJECT_TYPE type) {m_type = type;}

	bool deleteFlg(void) {return m_deleteFlg;}
	void deleteFlg(bool deleteFlg) {m_deleteFlg = deleteFlg;}

	void		objectList(ObjectList* list) {m_objectList = list;}
	ObjectBase*	objectPrev(void) {return m_objectPrev;}
	void		objectPrev(ObjectBase* prev) {m_objectPrev = prev;}
	ObjectBase*	objectNext(void) {return m_objectNext;}
	void		objectNext(ObjectBase* next) {m_objectNext = next;}

	void		updateList(UpdateList* list) {m_updateList = list;}
	ObjectBase*	updatePrev(void) {return m_updatePrev;}
	void		updatePrev(ObjectBase* prev) {m_updatePrev = prev;}
	ObjectBase*	updateNext(void) {return m_updateNext;}
	void		updateNext(ObjectBase* next) {m_updateNext = next;}

	void		drawList(DrawList* list, int shaderPatternId) {m_drawList[shaderPatternId] = list;}
	ObjectBase*	drawPrev(int shaderPatternId) {return m_drawPrev[shaderPatternId];}
	void		drawPrev(ObjectBase* prev, int shaderPatternId) {m_drawPrev[shaderPatternId] = prev;}
	ObjectBase*	drawNext(int shaderPatternId) {return m_drawNext[shaderPatternId];}
	void		drawNext(ObjectBase* next, int shaderPatternId) {m_drawNext[shaderPatternId] = next;}

protected:
	LPDIRECT3DDEVICE9 m_device;

	D3DXMATRIX	m_world;
	D3DXVECTOR3	m_pos;
	D3DXVECTOR3	m_rot;
	D3DXVECTOR3	m_scl;
	D3DXCOLOR	m_color;

	LPDIRECT3DTEXTURE9 m_texture;

private:
	int			m_priority;
	OBJECT_TYPE	m_type;
	bool		m_deleteFlg;

	ObjectList*	m_objectList;
	ObjectBase*	m_objectPrev;
	ObjectBase*	m_objectNext;

	UpdateList*	m_updateList;
	ObjectBase*	m_updatePrev;
	ObjectBase*	m_updateNext;

	DrawList*	m_drawList[Shader::PATTERN_MAX];
	ObjectBase*	m_drawPrev[Shader::PATTERN_MAX];
	ObjectBase*	m_drawNext[Shader::PATTERN_MAX];
};

//=============================================================================
#endif

// EOF
