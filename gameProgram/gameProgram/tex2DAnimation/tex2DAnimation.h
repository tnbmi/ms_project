//*****************************************************************************
//
// 2Dポリゴンアニメション [tex2DAnimation.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef TEX2DANIMATION_H_
#define TEX2DANIMATION_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Polygon2D;

class Tex2DAnimation
{
public:
	Tex2DAnimation(void);
	~Tex2DAnimation(void);

	static bool Create(Tex2DAnimation** outPointer);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	void Set2DPolygon( Polygon2D *poly ){ m_poly = poly; }
	void SetTexture( LPDIRECT3DTEXTURE9 tex ){ m_tex = tex; }
	void SetAnimationData( const int wait,const int animSum,
							const D3DXVECTOR2 &startUv0,const D3DXVECTOR2 &startUv1,
							const D3DXVECTOR2 &startUv2,const D3DXVECTOR2 &startUv3,
							const D3DXVECTOR2 &offset );

	void StartAnimaton( bool isRoop ){ m_time = 0; m_isRoop = isRoop; }
							

private:
	LPDIRECT3DTEXTURE9 m_tex;
	Polygon2D *m_poly;
	int m_time;
	int m_wait;
	int m_animSum;
	D3DXVECTOR2 m_startUv[4];
	D3DXVECTOR2 m_offset;
	bool m_isRoop;
};

//=============================================================================
#endif

// EOF