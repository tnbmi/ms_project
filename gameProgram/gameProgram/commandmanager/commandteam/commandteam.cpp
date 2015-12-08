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
#include "..\..\import\game\gameImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const float	_polygon_size_x = 50.0f;
const float	_polygon_pos_offset = 72.0f;
const float _polygon_speed_def = 2.0f;
typedef struct{
	float list[4];
}UV_LIST;
const UV_LIST	_comtexU_list[4] = 
{
	{0.5f, 0.625f, 0.5f, 0.625f},
	{0.625f, 0.75f, 0.625f, 0.75f},
	{0.75f, 0.875f, 0.75f, 0.875f},
	{0.875f, 1.0f, 0.875f, 1.0f},
};
const int	_command_data[4] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
};
typedef struct{
	float x;
	float y;
}POSITION_LIST;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CommandTeam::CommandTeam(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_command_pointer[0] = nullptr;
	m_command_pointer[1] = nullptr;
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
			m_command_data[i][j].hit = false;
			m_command_data[i][j].pos_y = 0.0f;
		}
	}

	m_command_count = 0;
	m_speed = 0;

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
						 GameImport* import,
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
							 GameImport* import,
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
bool CommandTeam::Update(void)
{
	// �|���S���X�N���[��
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			Polygon2D* p;
			p = m_command_data[i][j].polygon_pointer;
			if(m_command_data[i][j].polygon_pointer != nullptr)
				m_command_data[i][j].polygon_pointer->pos_y(m_command_data[i][j].polygon_pointer->pos().y + (_polygon_speed_def + m_speed));
		}
	}

	for(int i = 0; i < 2; i++)
	{
		if( m_pad[i]->buttonTrigger( 0x000f ) )
		{
			// �R�}���h����
		}
	}

	if(m_command_count >= COMMAND_MAX)
	{
		return true;
	}

	return false;
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
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(!Polygon2D::Create(&m_command_poly[i][j], m_device, m_objectList, m_import->texture(GameImport::COMMAND_TEX)))
				return false;
			m_updateList->Link(m_command_poly[i][j]);
			m_drawListManager->Link(m_command_poly[i][j], 3, Shader::PAT_2D);
			m_command_poly[i][j]->pos(0.0f, 0.0f, 0.0f);
			m_command_poly[i][j]->scl(_polygon_size_x, _polygon_size_x, 0.0f);
			for(int n = 0; n < 4; n++)
			{
				m_command_poly[i][j]->texcoord_u(n, _comtexU_list[0].list[n]);
			}
			m_command_poly[i][j]->color_a(0.0f);
		}
	}

	GameImport::TEX_TABLE texture = (GameImport::TEX_TABLE)(GameImport::BLUE_TEX + m_team_color);
	if(!Polygon2D::Create(&m_back_poly[0], m_device, m_objectList, m_import->texture(texture)))
		return false;
	m_updateList->Link(m_back_poly[0]);
	m_drawListManager->Link(m_back_poly[0], 2, Shader::PAT_2D);
	m_back_poly[0]->pos(m_polygon_pos.x + _polygon_pos_offset, m_polygon_pos.y - _polygon_pos_offset * 3, 0.0f);
	m_back_poly[0]->scl(_polygon_pos_offset * 2, _polygon_pos_offset * 6, 0.0f);

	if(!Polygon2D::Create(&m_back_poly[1], m_device, m_objectList, m_import->texture(GameImport::COMMAND_FRAM_00)))
		return false;
	m_updateList->Link(m_back_poly[1]);
	m_drawListManager->Link(m_back_poly[1], 4, Shader::PAT_2D);
	m_back_poly[1]->pos(m_polygon_pos.x + _polygon_pos_offset, m_polygon_pos.y - _polygon_pos_offset/2, 0.0f);
	m_back_poly[1]->scl(_polygon_pos_offset * 2 + 4.0f, _polygon_pos_offset + 4.0f, 0.0f);

	return true;
}

//=============================================================================
// �R�}���h�Z�b�g
//=============================================================================
void CommandTeam::SetCommand(unsigned int* command, unsigned int* nextCommand, int player)
{
	m_command_pointer[player] = command;
	m_command_pointer_Next[player] = nextCommand;

	for(int i = 0; i < 10; i++)
	{
		m_command_data[player][i].num_data = *(command + i);
		m_command_data[player][i].pos_y = 0.0f - _polygon_pos_offset * i;
	}

	SetPolygon(player);
}

//=============================================================================
// �|���S���Z�b�g
//=============================================================================
void CommandTeam::SetPolygon(int player)
{
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(m_command_data[player][i].num_data == 4)
				break;

			if(m_command_poly[player][j]->color().a == 0.0f)
			{
				m_command_data[player][i].polygon_pointer = m_command_poly[player][j];
				for(int n = 0; n < 4; n++)
				{
					m_command_data[player][i].polygon_pointer->texcoord_u(n, _comtexU_list[m_command_data[player][i].num_data].list[n]);
				}
				m_command_data[player][i].polygon_pointer->pos_x(m_polygon_pos.x + _polygon_pos_offset / 2 + player * _polygon_pos_offset );
				m_command_data[player][i].polygon_pointer->pos_y(m_command_data[player][i].pos_y);
				m_command_data[player][i].polygon_pointer->color_a(1.0f);
				break;
			}
		}
	}
}

// EOF
