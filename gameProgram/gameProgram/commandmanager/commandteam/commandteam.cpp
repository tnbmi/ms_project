//*****************************************************************************
//
// �`�[���̃R�}���h�Ǘ� [commandteam.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandTeam.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"

#include "..\commandmanager.h"

#include "..\..\input\padX\padX.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon2D\polygon2D.h"
#include "commandEffect\commandEffect.h"
#include "..\..\import\game\gameImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const float	_polygon_size_x = 50.0f;
const float	_polygon_pos_offset = 72.0f;
const float _polygon_speed_def = 2.0f;
const float _first_line = 144.0f;
const float _middlet_line = 120.5f;
const float _end_line = 72.0f;
const float _speed_max = 1.5f;
const float _speed_climax = 1.6f;
const float _speed_add = 0.04f;
const float _speed_rush_min = 0.5f;
const int _return_score1 = 10;
const int _return_score2 = 20;
const int	_polygon_num = 6;
const int	_effect_time = 30;
const int	_effect_max = 20;
const int	_effect_max_wait = 5;
const float	_add_scl_effect_success = 1.5f;
const float	_add_alpha_effect_success = -0.02f;
const float	_defalut_alpha_success = 0.7f;
const float	_add_alpha_effect_wait = -0.02f;
const float	_defalut_alpha_wait = 0.7f;
const int	_same_check_time = 16;
typedef struct{
	float list[4];
}UV_LIST;
const UV_LIST	_comtexU_list[5] = 
{
	{0.51f, 0.625f, 0.51f, 0.625f},
	{0.625f, 0.75f, 0.625f, 0.75f},
	{0.75f, 0.875f, 0.75f, 0.875f},
	{0.875f, 1.0f, 0.875f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
};
const int	_command_data[4] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CommandTeam::CommandTeam(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_command_pointer_Next[0] = nullptr;
	m_command_pointer_Next[1] = nullptr;

	m_pad[0] = nullptr;
	m_pad[1] = nullptr;

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			m_command_poly[i][j] = nullptr;
		}
	}

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			m_command_data[i][j].num_data = 0;
			m_command_data[i][j].polygon_pointer = nullptr;
			for(int num = 0; num < _effect_max; num++)
			{
				m_command_data[i][j].effect_pointer[num] = nullptr;
			}
			m_command_data[i][j].hit = false;
			m_command_data[i][j].pos_y = 0.0f;
			m_command_data[i][j].state = STATE_NONE;
			m_command_data[i][j].anime_count = 0;
		}
	}

	m_command_count = 0;
	m_delete_count = 0;
	m_same_count = 0;
	m_speed = 0.0f;
	m_offset = 0.0f;
	m_rushFlg = false;

	m_objectList		= nullptr;
	m_updateList		= nullptr;
	m_drawListManager	= nullptr;

	m_debugproc			= nullptr;
	m_device			= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CommandTeam::~CommandTeam(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool CommandTeam::Create(CommandTeam** outPointer,
						 ObjectList* objList,
						 UpdateList* updList,
						 DrawListManager* drwList,
						 LPDIRECT3DDEVICE9 device,
						 Import* import,
						 D3DXVECTOR3 pos,
						 TEAM_COLOR team_color)
{
	CommandTeam* pointer = new CommandTeam();
	if(!pointer->Initialize(objList, updList, drwList, device, import, pos, team_color))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool CommandTeam::Initialize(ObjectList* objList,
							 UpdateList* updList,
							 DrawListManager* drwList,
							 LPDIRECT3DDEVICE9 device,
							 Import* import,
							 D3DXVECTOR3 pos,
							 TEAM_COLOR team_color)
{
	//----------------------------
	// �X�e�[�^�X
	//----------------------------
	m_device = device;
	m_objectList = objList;
	m_updateList = updList;
	m_drawListManager = drwList;
	m_import = import;
	m_polygon_pos = pos;
	m_team_color = team_color;

	//----------------------------
	// �I�u�W�F�N�g
	//----------------------------
	if(!InitObject())
		return false;

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void CommandTeam::Finalize(void)
{

}

//=============================================================================
// �X�V
//=============================================================================
CommandTeam::COM_TEAM_RTN CommandTeam::Update(void)
{
	COM_TEAM_RTN rtn = {0,false,STATE_NONE};

	//----------------------------
	// �|���S���n�A�b�v�f�[�g
	//----------------------------
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 10; j++)
		{	// �|���S���X�N���[��
			m_command_data[i][j].pos_y += (_polygon_speed_def + m_speed);
			m_command_data[i][j].polygon_pointer->pos_y(m_command_data[i][j].pos_y);
			// �A�j���[�V��������
			switch(m_command_data[i][j].state)
			{
			// �R�}���h������
			case STATE_UP :
			case STATE_DOWN :
			case STATE_LEFT:
			case STATE_RIGHT:
			case STATE_DOUBLE:
				// �G�t�F�N�g�I�u�W�F�N�g�̍X�V���s��
				for(int num = 0; num < _effect_max; num++)
				{
					m_command_data[i][j].effect_pointer[num]->pos(m_command_data[i][j].polygon_pointer->pos());
					m_command_data[i][j].effect_pointer[num]->scl(m_command_data[i][j].effect_pointer[num]->scl().x + _add_scl_effect_success,
															 m_command_data[i][j].effect_pointer[num]->scl().y + _add_scl_effect_success,
															 0.0f);
					m_command_data[i][j].effect_pointer[num]->color_a(m_command_data[i][j].effect_pointer[num]->color().a + _add_alpha_effect_success);
				}
				// �A�j���[�V�����I��
				if(m_command_data[i][j].anime_count > _effect_time)
				{
					// �G�t�F�N�g�I�u�W�F�N�g�̉B��
					for(int num = 0; num < _effect_max; num++)
					{
						m_command_data[i][j].effect_pointer[num]->color_a(0.0f);
					}
					m_command_data[i][j].state = STATE_NONE;
				}
				// �A�j���[�V�����p�J�E���^
				m_command_data[i][j].anime_count++;
				break;
			// �R�}���h���s��
			case STATE_FAIL:
				m_command_data[i][j].polygon_pointer->scl(m_command_data[i][j].polygon_pointer->scl().x - 1.0f,
														  m_command_data[i][j].polygon_pointer->scl().y - 1.0f,
														  0.0f);
				m_command_data[i][j].anime_count++;
				if(m_command_data[i][j].anime_count > _effect_time)
					m_command_data[i][j].state = STATE_NONE;
				break;
			// ���������ҋ@��
			case STATE_WAIT:
				// �G�t�F�N�g�I�u�W�F�N�g�̍X�V���s��
				for(int num = 0; num < _effect_max_wait; num++)
				{
					m_command_data[i][j].effect_pointer[num]->pos(m_command_data[i][j].polygon_pointer->pos());
					m_command_data[i][j].effect_pointer[num]->color_a(m_command_data[i][j].effect_pointer[num]->color().a + _add_alpha_effect_wait);
					m_command_data[i][j].effect_pointer[num]->scl(m_command_data[i][j].effect_pointer[num]->scl().x - 0.25f,
																  m_command_data[i][j].effect_pointer[num]->scl().y - 0.25f,
																  0.0f);
				}
				break;
			default:
				break;
			}
		}
	}

	//----------------------------
	// �R�}���h����
	//----------------------------
	int command_count = m_command_count % 10;
	// ���������ҋ@����
	if(m_same_count > 0)
	{
		m_same_count++;
		// �P�\�t���[���I�[�o�[
		if(m_same_count >= _same_check_time)
		{
			m_same_count = 0;
			for(int i = 0; i < 2; i++)
			{
				m_command_data[i][command_count].polygon_pointer->color(0.75f, 0.75f, 0.75f, 0.5f);
				// �G�t�F�N�g�I�u�W�F�N�g�̉B��
				for(int num = 0; num < _effect_max; num++)
				{
					m_command_data[i][command_count].effect_pointer[num]->color_a(0.0f);
				}
				m_command_data[i][command_count].hit = true;
				m_command_data[i][command_count].state = STATE_FAIL;
				rtn.state = m_command_data[i][command_count].state;
			}
		}
	}
	// ���͔���
	for(int i = 0; i < 2; i++)
	{
		//----------------------------
		// �_�~�[�p�b�h�̍X�V
		//----------------------------
		if(m_pad[i]->dummy())
		{
			if(!m_command_data[i][command_count].hit && 
				m_command_data[i][command_count].pos_y > m_polygon_pos.y - _middlet_line)
				m_pad[i]->dummyPress();

			m_pad[i]->Update();
		}

		if( m_pad[i]->buttonTrigger( 0x000f ) )
		{
			if(!m_command_data[i][command_count].hit && 
				m_command_data[i][command_count].pos_y > m_polygon_pos.y - _first_line)
			{
				if(m_pad[i]->buttonTrigger(_command_data[m_command_data[i][command_count].num_data]))
				{	// ����(��������1�l��)
					if(m_command_data[(1 - i%2)][command_count].num_data != 4 && !m_command_data[(1 - i%2)][command_count].hit)
					{
						m_command_data[i][command_count].state = STATE_WAIT;
						// �G�t�F�N�g�I�u�W�F�N�g�̕\��
						for(int num = 0; num < _effect_max_wait; num++)
						{
							m_command_data[i][command_count].effect_pointer[num]->pos(m_command_data[i][command_count].polygon_pointer->pos());
							m_command_data[i][command_count].effect_pointer[num]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
							m_command_data[i][command_count].effect_pointer[num]->texture(m_import->texture(GameImport::EFFECT));
							for(int n = 0; n < 4; n++)
							{
								m_command_data[i][command_count].effect_pointer[num]->texcoord_u(n, _comtexU_list[4].list[n]);
							}
							m_command_data[i][command_count].effect_pointer[num]->color_a(_defalut_alpha_wait);
						}
						m_same_count = 1;
					}
					else if(m_same_count > 0)
					{// ����(��������2�l��)
						if(m_rushFlg)
							m_speed = _speed_climax;
						else
							m_speed += _speed_add;
						m_command_data[i][command_count].state = STATE_DOUBLE;
						m_command_data[1-(i%2)][command_count].state = STATE_DOUBLE;
						rtn.state = m_command_data[i][command_count].state;
						// 2�l���̃G�t�F�N�g�I�u�W�F�N�g�̕\��
						for(int num = 0; num < _effect_max; num++)
						{
							m_command_data[i][command_count].effect_pointer[num]->pos(m_command_data[i][command_count].polygon_pointer->pos());
							m_command_data[i][command_count].effect_pointer[num]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
							m_command_data[i][command_count].effect_pointer[num]->texture(m_import->texture(GameImport::COMMAND_TEX));
							m_command_data[1-(i%2)][command_count].effect_pointer[num]->pos(m_command_data[1-(i%2)][command_count].polygon_pointer->pos());
							m_command_data[1-(i%2)][command_count].effect_pointer[num]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
							m_command_data[1-(i%2)][command_count].effect_pointer[num]->texture(m_import->texture(GameImport::COMMAND_TEX));
							for(int n = 0; n < 4; n++)
							{
								m_command_data[i][command_count].effect_pointer[num]->texcoord_u(n, _comtexU_list[m_command_data[i][command_count].num_data].list[n]);
								m_command_data[1-(i%2)][command_count].effect_pointer[num]->texcoord_u(n, _comtexU_list[m_command_data[1-(i%2)][command_count].num_data].list[n]);
							}
							m_command_data[i][command_count].effect_pointer[num]->color_a(_defalut_alpha_success);
							m_command_data[1-(i%2)][command_count].effect_pointer[num]->color_a(_defalut_alpha_success);
						}
						m_same_count = 0;
						if(m_rushFlg)
						{
							if(m_speed > _speed_climax)
								m_speed = _speed_climax;
						}
						else
						{
							if(m_speed > _speed_max)
								m_speed = _speed_max;
						}
						rtn.return_score = _return_score2;
					}
					else
					{// ����(�P��)
						m_speed += _speed_add;
						m_command_data[i][command_count].state = (COMMAND_STATE)m_command_data[i][command_count].num_data;
						rtn.state = m_command_data[i][command_count].state;
						m_command_data[i][command_count].polygon_pointer->color(0.75f, 0.75f, 0.75f, 0.5f);
						// �G�t�F�N�g�I�u�W�F�N�g�̕\��
						for(int num = 0; num < _effect_max; num++)
						{
							m_command_data[i][command_count].effect_pointer[num]->pos(m_command_data[i][command_count].polygon_pointer->pos());
							m_command_data[i][command_count].effect_pointer[num]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
							m_command_data[i][command_count].effect_pointer[num]->texture(m_import->texture(GameImport::COMMAND_TEX));
							for(int n = 0; n < 4; n++)
							{
								m_command_data[i][command_count].effect_pointer[num]->texcoord_u(n, _comtexU_list[m_command_data[i][command_count].num_data].list[n]);
							}
							m_command_data[i][command_count].effect_pointer[num]->color_a(_defalut_alpha_success);
						}
						if(m_speed > _speed_max)
							m_speed = _speed_max;
						rtn.return_score = _return_score1;
					}
				}
				else
				{// ���s
					// ��������2�l�ڂŎ��s�������ꍇ
					if(m_same_count != 0)
					{
						m_same_count = 0;
						m_command_data[1-(i%2)][command_count].polygon_pointer->color(0.75f, 0.75f, 0.75f, 0.5f);
						for(int num = 0; num < _effect_max; num++)
						{
							m_command_data[1-(i%2)][command_count].effect_pointer[num]->color_a(0.0f);
						}
						m_command_data[1-(i%2)][command_count].state = STATE_FAIL;
					}
					if(m_rushFlg)
						m_speed = _speed_rush_min;
					else
						m_speed = 0.0f;
					m_command_data[i][command_count].polygon_pointer->color(0.75f, 0.75f, 0.75f, 0.5f);
					m_command_data[i][command_count].state = STATE_FAIL;
					rtn.state = m_command_data[i][command_count].state;
					// ��������1�l�ڎ��s�������ꍇ�A���������s��
					if(m_command_data[1-(i%2)][command_count].hit == false)
					{
						m_command_data[1-(i%2)][command_count].polygon_pointer->color(0.75f, 0.75f, 0.75f, 0.5f);
						m_command_data[1-(i%2)][command_count].state = STATE_FAIL;
						m_command_data[1-(i%2)][command_count].hit = true;
					}

				}
				m_command_data[i][command_count].hit = true;
			}
		}
	}
	// �X�N���[���A�E�g����
	if(m_command_data[0][command_count].pos_y > m_polygon_pos.y - _end_line)
	{
		m_command_count++;
		m_same_count = 0;

		for(int i = 0; i < 2; i++)
		{
			if(!m_command_data[i][command_count].hit)
			{
				m_speed = 0.0f;
				m_command_data[i][command_count].state = STATE_FAIL;
				rtn.state = m_command_data[i][command_count].state;
			}

			if(m_pad[i]->dummy())
				m_pad[i]->commandCnt(m_command_count % 10);
		}
	}

	// �R�}���h�f�[�^�X�V
	int delete_count = m_delete_count % 10;
	if(m_command_data[0][delete_count].pos_y > m_polygon_pos.y - _polygon_pos_offset / 2)
	{
		for(int i = 0; i < 2; i++)
		{
			// �f�[�^�����R�}���h����ǂݍ���
			m_command_data[i][delete_count].num_data = *(m_command_pointer_Next[i] + delete_count);
			m_command_data[i][delete_count].pos_y = m_command_data[i][delete_count].pos_y - m_offset * COMMAND_MAX;
			m_command_data[i][delete_count].hit = false;
			// �G�t�F�N�g�I�u�W�F�N�g�̉B��(�O�̂���)
			for(int num = 0; num < _effect_max; num++)
			{
				m_command_data[i][delete_count].effect_pointer[num]->color_a(0.0f);
			}
			// �|���S��������
			if(m_command_data[i][delete_count].num_data == 4)
			{
				m_command_data[i][delete_count].hit = true;
				m_command_data[i][delete_count].polygon_pointer->color_a(0.0f);
			}
			else
			{
				for(int n = 0; n < 4; n++)
				{
					m_command_data[i][delete_count].polygon_pointer->texcoord_u(n, _comtexU_list[m_command_data[i][delete_count].num_data].list[n]);
				}
				m_command_data[i][delete_count].polygon_pointer->pos_x(m_polygon_pos.x + _polygon_pos_offset / 2 + i * _polygon_pos_offset );
				m_command_data[i][delete_count].polygon_pointer->pos_y(m_command_data[i][delete_count].pos_y);
				m_command_data[i][delete_count].polygon_pointer->color(1.0f, 1.0f, 1.0f, 1.0f);
				m_command_poly[i][delete_count]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
				m_command_data[i][delete_count].state = STATE_NONE;
				m_command_data[i][delete_count].anime_count = 0;
			}
		}
		m_delete_count++;
	}

	// ���|�C���^�v������
	if(m_delete_count != 0 && m_delete_count % COMMAND_MAX == 0)
	{
		rtn.flag = true;
		m_delete_count = 0;
	}
	return rtn;
}

//=============================================================================
// �`��
//=============================================================================
void CommandTeam::Draw(void)
{
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
bool CommandTeam::InitObject(void)
{
	//----------------------------
	// 2D�|���S��
	//----------------------------
	// �R�}���h�|���S���p�I�u�W�F�N�g
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(!Polygon2D::Create(&m_command_poly[i][j], m_device, m_objectList, m_import->texture(GameImport::COMMAND_TEX)))
				return false;
			m_updateList->Link(m_command_poly[i][j]);
			m_drawListManager->Link(m_command_poly[i][j], 2, Shader::PAT_2D);
			m_command_poly[i][j]->pos(0.0f, 0.0f, 0.0f);
			m_command_poly[i][j]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
			for(int n = 0; n < 4; n++)
			{
				m_command_poly[i][j]->texcoord_u(n, _comtexU_list[0].list[n]);
			}
			m_command_poly[i][j]->color_a(0.0f);
		}
	}

	// �R�}���h�G�t�F�N�g�p�I�u�W�F�N�g
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			for(int num = 0; num < _effect_max; num++)
			{
				if(!CommandEffect::Create(&m_command_data[i][j].effect_pointer[num], m_device, m_objectList, m_import->texture(GameImport::EFFECT)))
					return false;
				m_updateList->Link(m_command_data[i][j].effect_pointer[num]);
				m_drawListManager->Link(m_command_data[i][j].effect_pointer[num], 3, Shader::PAT_2D);
				m_command_data[i][j].effect_pointer[num]->pos(0.0f, 0.0f, 0.0f);
				m_command_data[i][j].effect_pointer[num]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
				m_command_data[i][j].effect_pointer[num]->color_a(0.0f);
			}
		}
	}

	// �w�i���C���p�I�u�W�F�N�g
	GameImport::TEX_TABLE texture = (GameImport::TEX_TABLE)(GameImport::BLUE_TEX + m_team_color);
	if(!Polygon2D::Create(&m_back_poly[0], m_device, m_objectList, m_import->texture(texture)))
		return false;
	m_updateList->Link(m_back_poly[0]);
	m_drawListManager->Link(m_back_poly[0], 1, Shader::PAT_2D);
	m_back_poly[0]->pos(m_polygon_pos.x + _polygon_pos_offset, m_polygon_pos.y - _polygon_pos_offset * 3, 0.0f);
	m_back_poly[0]->scl(_polygon_pos_offset * 2, _polygon_pos_offset * 6, 0.0f);

	// �t���[���E�B���h�E�p�I�u�W�F�N�g
	if(!Polygon2D::Create(&m_back_poly[1], m_device, m_objectList, m_import->texture(GameImport::COMMAND_FRAM_00)))
		return false;
	m_updateList->Link(m_back_poly[1]);
	m_drawListManager->Link(m_back_poly[1], 4, Shader::PAT_2D);
	m_back_poly[1]->pos(m_polygon_pos.x + _polygon_pos_offset, m_polygon_pos.y - _polygon_pos_offset/2 - _polygon_pos_offset, 0.0f);
	m_back_poly[1]->scl(_polygon_pos_offset * 2 + 4.0f, _polygon_pos_offset + 4.0f, 0.0f);

	return true;
}

//=============================================================================
// �R�}���h�Z�b�g
//=============================================================================
void CommandTeam::SetCommand(unsigned int* command, unsigned int* nextCommand, int player, float offset)
{
	m_command_pointer_Next[player] = nextCommand;
	m_offset = offset;

	for(int i = 0; i < COMMAND_MAX; i++)
	{
		m_command_data[player][i].num_data = *(command + i);
		m_command_data[player][i].pos_y = 0.0f - m_offset * i;
		m_command_data[player][i].polygon_pointer = m_command_poly[player][i];
		if(m_command_data[player][i].num_data == 4)
			m_command_data[player][i].hit = true;
	}

	if(m_pad[player]->dummy())
		m_pad[player]->commandPrev(m_commandPrev);

	SetPolygon(player);
}

//=============================================================================
// ���R�}���h�Z�b�g
//=============================================================================
void CommandTeam::SetCommandNext(unsigned int* command, int player, float offset)
{
	m_command_pointer_Next[player] = command;
	if(offset != 0.0f)
		m_offset = offset;

	if(m_pad[player]->dummy())
		m_pad[player]->commandPrev(m_commandPrev);
}

//=============================================================================
// �|���S���Z�b�g
//=============================================================================
void CommandTeam::SetPolygon(int player)
{
	for(int i = 0; i < COMMAND_MAX; i++)
	{
		if(m_command_data[player][i].num_data != 4)
		{
			for(int n = 0; n < 4; n++)
			{
				m_command_data[player][i].polygon_pointer->texcoord_u(n, _comtexU_list[m_command_data[player][i].num_data].list[n]);
			}
			m_command_data[player][i].polygon_pointer->pos_x(m_polygon_pos.x + _polygon_pos_offset / 2 + player * _polygon_pos_offset );
			m_command_data[player][i].polygon_pointer->pos_y(m_command_data[player][i].pos_y);
			m_command_data[player][i].polygon_pointer->color_a(1.0f);
		}
	}
}

// EOF
