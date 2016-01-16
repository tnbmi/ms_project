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
#include "..\common\random\random.h"

#include "..\input\padX\padXManager.h"
#include "..\input\padX\padX.h"
#include "..\input\padX\dummyPadX.h"

#include "..\debugproc\debugproc.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\import\game\gameImport.h"

#include "commandData\commandDataLoad.h"

#include "..\objectBase\polygon2D\polygon2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char* _command_data = "./resources/command/comDataTest.txt";
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
	m_command_prev[0] = 0;
	m_command_prev[1] = 0;
	m_ui_polygon = nullptr;
	m_objectList = nullptr;
	m_updateList = nullptr;
	m_drawListManager = nullptr;

	m_demoFlg = false;
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
							Import* import,
							bool demoFlg)
{
	CommandManager* pointer = new CommandManager();
	if(!pointer->Initialize(padXManager, debugproc, objList, updList, drwList, device, import, demoFlg))
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
								Import* import,
								bool demoFlg)
{
	m_objectList = objList;
	m_updateList = updList;
	m_drawListManager = drwList;

	m_debugproc = debugproc;
	m_import = import;

	m_demoFlg = demoFlg;

	//----------------------------
	// �R�}���h�̓ǂݍ���
	//----------------------------
	CommandDataLoad::Create(&m_commandDataLoad, _command_data, _list_pattern_max);

	m_command_list[0] = m_commandDataLoad->commandDataA();
	m_command_list[1] = m_commandDataLoad->commandDataB();

	//----------------------------
	// �`�[������
	//----------------------------
	for(int i = 0; i < _team_max; i++)
	{
		CommandTeam::Create(&m_team[i], m_objectList, m_updateList, m_drawListManager, device, import, _team_position[i], (CommandTeam::TEAM_COLOR)_team_color[i]);
		m_team[i]->debugproc(debugproc);

		//----------------------------
		// �p�b�h�ݒ�
		//----------------------------
		if(m_demoFlg)
		{
			DummyPadX::Create(&m_pad[i*2], i*2, _list_pattern_max, m_command_list[0], Random::Rand(75, 97));
			m_pad[i*2]->debugproc(m_debugproc);

			DummyPadX::Create(&m_pad[i*2+1], i*2+1, _list_pattern_max, m_command_list[1], Random::Rand(75, 97));
			m_pad[i*2+1]->debugproc(m_debugproc);
		}
		else
		{
			if(padXManager->pad(i*2)->conected())
				m_pad[i*2] = padXManager->pad(i*2);
			else
			{
				DummyPadX::Create(&m_pad[i*2], i*2, _list_pattern_max, m_command_list[0], Random::Rand(75, 97));
				m_pad[i*2]->debugproc(m_debugproc);
			}

			if(padXManager->pad(i*2+1)->conected())
				m_pad[i*2+1] = padXManager->pad(i*2+1);
			else
			{
				DummyPadX::Create(&m_pad[i*2+1], i*2+1, _list_pattern_max, m_command_list[1], Random::Rand(75, 97));
				m_pad[i*2+1]->debugproc(m_debugproc);
			}
		}

		m_team[i]->SetPlayer( m_pad[i*2], m_pad[i*2+1] );
		//if( i == 0 )
		//{
		//	m_team[i]->SetPlayer( pad[i], pad[i+1] );
		//}
		//else if( i == 1 )
		//{
		//	m_team[i]->SetPlayer( pad[i+1], pad[i+1] );
		//}
		//m_team[i]->SetPlayer( pad[i], pad[i] );
		//m_team[i]->SetPlayer( pad[i*2], pad[i*2+1] );

		//----------------------------
		// �R�}���h�ݒ�
		//----------------------------
		int num = Random::Rand()%10;
		m_team[i]->commandPrev(num);
		m_command_prev[i] = (Random::Rand() % 5 * 2) + (1 - num % 2);
		m_team[i]->SetCommand(m_command_list[0] + num * 10, m_command_list[0] + m_command_prev[i] * 10, 0, 144.0f);
		m_team[i]->SetCommand(m_command_list[1] + num * 10, m_command_list[1] + m_command_prev[i] * 10, 1, 144.0f);
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

		if(m_pad[i*2]->dummy())
			SafeFinalizeDelete(m_pad[i*2]);

		if(m_pad[i*2+1]->dummy())
			SafeFinalizeDelete(m_pad[i*2+1]);
	}

	SafeFinalizeDelete(m_commandDataLoad);
}

//=============================================================================
// �X�V
//=============================================================================
CommandManager::COM_MANA_RTN CommandManager::Update(void)
{
	CommandTeam::COM_TEAM_RTN get = {0, false, CommandTeam::STATE_NONE};

	COM_MANA_RTN rtn = {0,0,CommandTeam::STATE_NONE,CommandTeam::STATE_NONE};
	for(int i = 0; i < _team_max; i++)
	{
		get = m_team[i]->Update();
		rtn.score[i] = get.return_score;
		rtn.state[i] = get.state;
		if(get.flag)
		{
			m_team[i]->commandPrev(m_command_prev[i]);
			m_command_prev[i] = (Random::Rand() % 5 * 2) + (1 - m_command_prev[i] % 2);
			m_team[i]->SetCommandNext(m_command_list[0] + m_command_prev[i] * 10, 0);
			m_team[i]->SetCommandNext(m_command_list[1] + m_command_prev[i] * 10, 1);
		}
	}
	return rtn;
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