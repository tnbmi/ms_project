//*****************************************************************************
//
// �R�}���h�`�F�b�N�����}�l�[�W�� [commandmanager.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandManager.h"
#include "..\common\safe.h"

#include "..\input\padX\padXManager.h"

#include "..\debugproc\debugproc.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\import\game\gameImport.h"

#include "commandteam\commandteam.h"
#include "commandData\commandDataLoad.h"

#include "..\objectBase\polygon2D\polygon2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _team_max = 2;
const int _list_pattern_max = 10;
const D3DXVECTOR3 _team_position[2] = {D3DXVECTOR3(24.0f, 482.0f, 0.0f),D3DXVECTOR3(1112.0f, 482.0f, 0.0f)};
const int _team_color[2]= {CommandTeam::COLOR_BLUE,CommandTeam::COLOR_RED};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CommandManager::CommandManager(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_commandDataLoad = nullptr;
	for(int i = 0; i < _team_max; i++)
	{
		m_team[i] = nullptr;
	}
	m_command_list[0] = nullptr;
	m_command_list[1] = nullptr;
	m_command_prev = 0;
	m_ui_polygon = nullptr;
	m_objectList = nullptr;
	m_updateList = nullptr;
	m_drawListManager = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CommandManager::~CommandManager(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool CommandManager::Create(CommandManager** outPointer,
							PadXManager* padXManager,
							Debugproc* debugproc,
							ObjectList* objList,
							UpdateList* updList,
							DrawListManager* drwList,
							LPDIRECT3DDEVICE9 device,
							GameImport* import)
{
	CommandManager* pointer = new CommandManager();
	if(!pointer->Initialize(padXManager, debugproc, objList, updList, drwList, device, import))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool CommandManager::Initialize(PadXManager* padXManager,
								Debugproc* debugproc,
								ObjectList* objList,
								UpdateList* updList,
								DrawListManager* drwList,
								LPDIRECT3DDEVICE9 device,
								GameImport* import)
{
	//----------------------------
	// �R�����g
	//----------------------------
	m_objectList = objList;
	m_updateList = updList;
	m_drawListManager = drwList;

	m_debugproc = debugproc;
	m_import = import;

	//----------------------------
	// �R�}���h�̓ǂݍ���
	//----------------------------
	CommandDataLoad::Create(&m_commandDataLoad, _list_pattern_max);

	m_command_list[0] = m_commandDataLoad->commandDataLoadA();
	m_command_list[1] = m_commandDataLoad->commandDataLoadB();

	for(int i = 0; i < _team_max; i++)
	{
		CommandTeam::Create(&m_team[i], m_objectList, m_updateList, m_drawListManager, device, import, _team_position[i], (CommandTeam::TEAM_COLOR)_team_color[i]);
		m_team[i]->debugproc(debugproc);
#ifdef _DEBUG
		m_team[i]->SetPlayer( padXManager->pad(i), padXManager->pad(i) );
		//m_team[i]->SetPlayer( padXManager->pad(i * 2), padXManager->pad(i * 2 + 1) );
#else
		m_team[i]->SetPlayer( padXManager->pad(i * 2), padXManager->pad(i * 2 + 1) );
#endif
		m_team[i]->SetCommand(m_command_list[0], m_command_list[0], 0, 144.0f);
		m_team[i]->SetCommand(m_command_list[1], m_command_list[1], 1, 144.0f);
	}

	if(!Polygon2D::Create(&m_ui_polygon, device, m_objectList, m_import->texture(GameImport::GAME_UI)))
		return false;
	m_updateList->Link(m_ui_polygon);
	m_drawListManager->Link(m_ui_polygon, 3, Shader::PAT_2D);
	m_ui_polygon->pos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f);
	m_ui_polygon->scl(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void CommandManager::Finalize(void)
{
	for(int i = 0; i < _team_max; i++)
	{
		SafeFinalizeDelete(m_team[i]);
	}

	SafeFinalizeDelete(m_commandDataLoad);
}

//=============================================================================
// �X�V
//=============================================================================
void CommandManager::Update(void)
{
	for(int i = 0; i < _team_max; i++)
	{
		if(m_team[i]->Update())
		{
			m_command_prev++;
			m_team[i]->SetCommandNext(m_command_list[0] + m_command_prev * 10, 0);
			m_team[i]->SetCommandNext(m_command_list[1] + m_command_prev * 10, 1);
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void CommandManager::Draw(void)
{
	for(int i = 0; i < _team_max; i++)
	{
		m_team[i]->Draw();
	}
}


// EOF