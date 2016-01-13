//*****************************************************************************
//
// ソースのテンプレート [Tex2DAnimation.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "tex2DAnimation.h"
#include "..\objectBase\polygon2D\polygon2D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Tex2DAnimation::Tex2DAnimation(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_tex = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Tex2DAnimation::~Tex2DAnimation(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Tex2DAnimation::Create(Tex2DAnimation** outPointer)
{
	Tex2DAnimation* pointer = new Tex2DAnimation();

	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Tex2DAnimation::Initialize(void)
{
	//----------------------------
	// コメント
	//----------------------------
	m_isRoop = false;
	m_isReverse = false;
	m_valNum = 1;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Tex2DAnimation::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void Tex2DAnimation::Update(void)
{
	int animNum = (int)( m_time / m_wait ) % m_animSum;
	D3DXVECTOR2 uv[4];

	D3DXVECTOR2 offset = m_offset * (float)animNum;

	uv[0] = m_startUv[0] + offset;
	uv[1] = m_startUv[1] + offset;
	uv[2] = m_startUv[2] + offset;
	uv[3] = m_startUv[3] + offset;
	
	m_time+= m_valNum;
	
	
	if( ((int)( m_time / m_wait ) >= m_animSum) || ((int)( m_time / m_wait ) < 0) )
	{
		if( m_isRoop )
		{
			m_isEndAnimation = false;

			if( m_isReverse )
			{
				m_valNum*=-1;
				m_time+= m_valNum;
			}
			else
			{
				m_time = 0;
			}
		}
		else
		{
			m_time -=m_valNum;
			m_isEndAnimation = true;
		}
	}
	
	
	m_poly->texcoord(0,uv[0].x,uv[0].y );
	m_poly->texcoord(1,uv[1].x,uv[1].y );
	m_poly->texcoord(2,uv[2].x,uv[2].y );
	m_poly->texcoord(3,uv[3].x,uv[3].y );
}

//=============================================================================
// 描画
//=============================================================================
void Tex2DAnimation::Draw(void)
{
}

//=============================================================================
//
//=============================================================================

void Tex2DAnimation::SetAnimationData( const float wait,const int animSum,
							const D3DXVECTOR2 &startUv0,const D3DXVECTOR2 &startUv1,
							const D3DXVECTOR2 &startUv2,const D3DXVECTOR2 &startUv3,
							const D3DXVECTOR2 &offset )
{
	m_animSum = animSum;
	m_wait = wait;
	m_startUv[0] = startUv0;
	m_startUv[1] = startUv1;
	m_startUv[2] = startUv2;
	m_startUv[3] = startUv3;
	m_offset = offset;
	m_isEndAnimation = false;
}

//=============================================================================
//
//=============================================================================
void Tex2DAnimation::SetTextureToPolygon(void)
{
	m_poly->texture(m_tex);
}

// EOF
