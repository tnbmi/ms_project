//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [fade.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "fade.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
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
// �R���X�g���N�^
//=============================================================================
Fade::Fade(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : Polygon2D(device,objectList,type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_state = FADESTATE_NONE;
	m_time	= 0;
	m_alpha = 0.0f;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_flg	= false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Fade::~Fade(void)
{
}

//=============================================================================
// ����
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
// ������
//=============================================================================
bool Fade::Initialize(void)
{
	//----------------------------
	// ������
	//----------------------------
	Polygon2D::Initialize( nullptr );
	m_scl = D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f );
	pos( SCREEN_WIDTH*0.5f , SCREEN_HEIGHT*0.5f , 0.0f );
	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Fade::Finalize(void)
{
	Polygon2D::Finalize();
}

//=============================================================================
// �X�V
//=============================================================================
void Fade::Update(void)
{
	//----------------------------
	// �t�F�[�h����
	//----------------------------
	if( m_flg && m_time >= 0 )
	{
		// �A���t�@�l�ύX
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

		// �J�E���g�_�E��
		m_time--;
		if( m_time < 0 )
		{
			// ���[�h�X�V
			m_state = _fadeNext[m_state];
			m_flg = false;
		}
	}
	else if( m_state == FADESTATE_INEND )
	{
		m_state = FADESTATE_NONE;
	}
	//----------------------------
	// �e�N���X�X�V
	//----------------------------
	Polygon2D::Update();
}

//=============================================================================
// �`��
//=============================================================================
void Fade::Draw(void)
{
}
//=============================================================================
// �J�n
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
