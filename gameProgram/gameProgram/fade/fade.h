//*****************************************************************************
//
// ソースのテンプレート [fade.h]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

#ifndef FADE_H_
#define FADE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\objectBase\polygon2D\polygon2D.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Fade : public Polygon2D
{
public:

	typedef enum
	{
		FADESTATE_NONE = 0,
		FADESTATE_OUT,
		FADESTATE_OUTEND,
		FADESTATE_IN,
		FADESTATE_INEND,
		FADESTATE_MAX
	}FADESTATE;

	Fade(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type);
	virtual ~Fade(void);

	static bool Create(Fade** outPointer , LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	void Start(FADESTATE state, int time, float r, float g, float b, float a);

private:
	FADESTATE m_state;
	int m_time;
	float m_alpha;
	D3DXCOLOR m_color;
	float m_cngAlpha;
	bool m_flg;
};

//=============================================================================
#endif

// EOF
