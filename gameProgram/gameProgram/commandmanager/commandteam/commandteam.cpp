//*****************************************************************************
//
// �`�[���̃R�}���h�Ǘ� [commandteam.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandteam.h"
#include "..\..\debugproc\debugproc.h"
#include "..\..\input\padX\padX.h"

#define TIME_PENALTY (30)
#define COMMAND_MAX (6)
#define COMMAND_MIN (4)

const char* _comtex[6] = {"UP\n","Y\n","RIGHT\n","DOWN\n","A\n","X\n"};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Commandteam::Commandteam(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_command_list[0] = XINPUT_GAMEPAD_DPAD_UP;
	m_command_list[1] = XINPUT_GAMEPAD_Y;
	m_command_list[2] = XINPUT_GAMEPAD_DPAD_RIGHT;
	m_command_list[3] = XINPUT_GAMEPAD_DPAD_DOWN;
	m_command_list[4] = XINPUT_GAMEPAD_A;
	m_command_list[5] = XINPUT_GAMEPAD_X;

	m_pad[0]		= nullptr;
	m_pad[1]		= nullptr;
	m_debugproc		= nullptr;

	m_command_count = 0;
	m_time_penalty = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Commandteam::~Commandteam(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Commandteam::Create(Commandteam** outPointer)
{
	Commandteam* pointer = new Commandteam();
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Commandteam::Initialize(void)
{
	//----------------------------
	// �R�����g
	//----------------------------

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Commandteam::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void Commandteam::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc( _comtex[m_command_count] );
#endif

	if(m_time_penalty > 0)		// �y�i���e�B�^�C��
	{
		m_time_penalty--;
#ifdef _DEBUG
		m_debugproc->PrintDebugProc( "ERROR!\n" );
#endif
	}
	else						// �ʏ폈��
	{
		int current_user = m_command_count % 2;

		if( m_pad[current_user]->buttonTrigger( 0xf00f ) )
		{
			if( m_pad[current_user]->buttonTrigger( m_command_list[m_command_count] ) )
				m_command_count++;
			else
				m_time_penalty = TIME_PENALTY;
		}
	}

	if(m_command_count >= COMMAND_MAX)
		m_command_count = 0;
}

//=============================================================================
// �`��
//=============================================================================
void Commandteam::Draw(void)
{
}

// EOF
