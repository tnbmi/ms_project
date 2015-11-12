//*****************************************************************************
//
// �R�}���h�`�F�b�N���� [commandmanager.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandmanager.h"
#include "..\input\keyboard\keyboard.h"
#include "..\debugproc\debugproc.h"

#define TIME_PENALTY (30)
const char* _comtex[6] = {"UP\n","Y\n","RIGHT\n","DOWN\n","A\n","X\n"};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Commandmanager::Commandmanager(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_command_list[0] = DIK_UP;
	m_command_list[1] = DIK_Y;
	m_command_list[2] = DIK_RIGHT;
	m_command_list[3] = DIK_DOWN;
	m_command_list[4] = DIK_A;
	m_command_list[5] = DIK_X;
	m_keyboard		= nullptr;
	m_debugproc		= nullptr;
	m_command_count = 0;
	m_time_penalty = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Commandmanager::~Commandmanager(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Commandmanager::Create(Commandmanager** outPointer)
{
	Commandmanager* pointer = new Commandmanager();
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Commandmanager::Initialize(void)
{
	//----------------------------
	// �R�����g
	//----------------------------

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Commandmanager::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void Commandmanager::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc( _comtex[m_command_count] );
#endif

	// �y�i���e�B�^�C��
	if(m_time_penalty > 0)
	{
		m_time_penalty--;
#ifdef _DEBUG
		m_debugproc->PrintDebugProc( "ERROR!" );
#endif
	}
	else
	{
		if( m_keyboard->trigger( DIK_UP ) ||				// Pad(0xf00f)
			m_keyboard->trigger( DIK_DOWN ) ||
			m_keyboard->trigger( DIK_RIGHT ) || 
			m_keyboard->trigger( DIK_LEFT ) || 
			m_keyboard->trigger( DIK_A ) || 
			m_keyboard->trigger( DIK_B ) || 
			m_keyboard->trigger( DIK_X ) || 
			m_keyboard->trigger( DIK_Y ) )
		{
			if( m_keyboard->trigger( m_command_list[m_command_count] ) )
				m_command_count++;
			else
				m_time_penalty = TIME_PENALTY;
		}
	}
	if(m_command_count >= 6)
		m_command_count = 0;
}

//=============================================================================
// �`��
//=============================================================================
void Commandmanager::Draw(void)
{
}

// EOF