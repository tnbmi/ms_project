//*****************************************************************************
//
// ソースのテンプレート [fade.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "fade.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _fadeCng[Fade::FADESTATE_MAX] =
{
	0,
	1,
	0,
	-1,
	0
};
const Fade::FADESTATE _fadeNext[Fade::FADESTATE_MAX] =
{
	Fade::FADESTATE_NONE,
	Fade::FADESTATE_OUTEND,
	Fade::FADESTATE_NONE,
	Fade::FADESTATE_INEND,
	Fade::FADESTATE_NONE
};
//=============================================================================
// コンストラクタ
//=============================================================================
Fade::Fade(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : Polygon2D(device,objectList,type)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_state = FADESTATE_NONE;
	m_time	= 0;
	m_alpha = 0.0f;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_flg	= false;
}

//=============================================================================
// デストラクタ
//=============================================================================
Fade::~Fade(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Fade::Create(Fade** outPointer , LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type )
{
	Fade* pointer = new Fade( device , objectList , type);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Fade::Initialize(void)
{
	//----------------------------
	// 初期化
	//----------------------------
	Polygon2D::Initialize( nullptr );
	m_scl = D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f );
	pos( SCREEN_WIDTH*0.5f , SCREEN_HEIGHT*0.5f , 0.0f );
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Fade::Finalize(void)
{
	Polygon2D::Finalize();
}

//=============================================================================
// 更新
//=============================================================================
void Fade::Update(void)
{
	//----------------------------
	// フェード処理
	//----------------------------
	if( m_flg && m_time >= 0 )
	{
		// アルファ値変更
		m_alpha += m_cngAlpha * _fadeCng[m_state];
		if( m_alpha < 0.0f )
		{
			m_alpha = 0.0f;
		}
		else if( m_alpha > 1.0f )
		{
			m_alpha = 1.0f;
		}

		m_color = D3DXCOLOR( m_color.r , m_color.g , m_color.b , m_alpha );
		Polygon2D::color( m_color.r, m_color.g , m_color.b , m_color.a );

		// カウントダウン
		m_time--;
		if( m_time < 0 )
		{
			// モード更新
			m_state = _fadeNext[m_state];
			m_flg = false;
		}
	}
	else if( m_state == FADESTATE_INEND )
	{
		m_state = FADESTATE_NONE;
	}
	//----------------------------
	// 親クラス更新
	//----------------------------
	Polygon2D::Update();
}

//=============================================================================
// 描画
//=============================================================================
void Fade::Draw(void)
{
}
//=============================================================================
// 開始
//=============================================================================
void Fade::Start(FADESTATE state, int time, float r, float g, float b, float a)
{
	m_state		= state;
	m_time		= time * 60;
	m_alpha		= a;
	m_color		= D3DXCOLOR(r, g, b, a);
	m_cngAlpha	= 1.0f / (float)m_time;
	Polygon2D::color( m_color.r , m_color.g , m_color.b , m_alpha );

	if(state == FADESTATE_IN || state == FADESTATE_OUT)
	{
		m_flg = true;
	}

}

// EOF
