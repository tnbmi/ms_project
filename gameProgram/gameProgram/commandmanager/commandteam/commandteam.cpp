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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int	_time_penalty = 30;
const int	_command_max = 6;
const int	_command_min = 4;
const int	_upper_limit = 2;
const char*	_comtex[6] = {"UP\n","Y\n","RIGHT\n","DOWN\n","A\n","X\n"};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Commandteam::Commandteam(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	/*for(int i = 0; i < COMMAND_MAX; i++)
	{
		m_command_list[i] = 0;
	}*/
	m_command = nullptr;

	m_pad[0]		= nullptr;
	m_pad[1]		= nullptr;
	m_debugproc		= nullptr;

	m_command_count = 0;
	m_time_penalty = 0;
	m_command_long = 0;
	m_flag_lose = false;
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
	m_command_long = _command_max;

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
bool Commandteam::Update(void)
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
			if( m_pad[current_user]->buttonTrigger( *(m_command + m_command_count) ) )
				m_command_count++;
			else
				m_time_penalty = _time_penalty;
		}
	}

	if(m_command_count >= m_command_long)
	{
		m_command_count = 0;

		if(m_flag_lose)
			m_command_long = _command_min;

		return true;
	}

	return false;
}

//=============================================================================
// �`��
//=============================================================================
void Commandteam::Draw(void)
{
}

//=============================================================================
// �t���O�Z�b�g
//=============================================================================
void Commandteam::SetFragLose(bool flag)
{
	m_flag_lose = flag;

	if(flag)
	{
		if(m_command_count <= _upper_limit)
		{
			m_command_long = _command_min;
		}
	}
}

// EOF