//*****************************************************************************
//
// コマンドチェック処理マネージャ [commandmanager.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandManager.h"
#include "..\common\safe.h"

#include "..\input\padX\padXManager.h"

#include "..\debugproc\debugproc.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\import\game\gameImport.h"

#include "..\commandmanager\commandteam\commandteam.h"

#include "..\objectBase\polygon2D\polygon2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _team_max = 2;
const int _progress_neutral = 5;
const int _progress_team0_lead = 3;
const int _progress_team1_lead = 7;
const int _list_pattern_max = 40;
const D3DXVECTOR3 _team_position[2] = {D3DXVECTOR3(48.0f, 432.0f, 0.0f),D3DXVECTOR3(1088.0f, 432.0f, 0.0f)};
const int _team_color[2]= {CommandTeam::COLOR_BLUE,CommandTeam::COLOR_RED};

//=============================================================================
// コンストラクタ
//=============================================================================
CommandManager::CommandManager(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	for(int i = 0; i < _team_max; i++)
	{
		m_team[i] = nullptr;
	}
	m_progress = 0;
	m_command_list = nullptr;
	m_objectList = nullptr;
	m_updateList = nullptr;
	m_drawListManager = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CommandManager::~CommandManager(void)
{
}

//=============================================================================
// 生成
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
// 初期化
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
	// コメント
	//----------------------------
	m_progress = _progress_neutral;

	m_objectList = objList;
	m_updateList = updList;
	m_drawListManager = drwList;

	m_debugproc = debugproc;
	m_game_state = DRAW;
	m_import = import;

	m_command_list = new unsigned int [_list_pattern_max * COMMAND_MAX];

	//---------------------------------------------------------------------------------------------
	// 後でデータロードに置き換えるー
	//---------------------------------------------------------------------------------------------
	for(int i = 0; i < _list_pattern_max * COMMAND_MAX; i++)
	{
		m_command_list[i] = rand()%4;
	}

	for(int i = 0; i < _team_max; i++)
	{
		CommandTeam::Create(&m_team[i], m_objectList, m_updateList, m_drawListManager, device, import, _team_position[i], (CommandTeam::TEAM_COLOR)_team_color[i]);
		m_team[i]->debugproc(debugproc);
#ifdef _DEBUG
		m_team[i]->SetPlayer( padXManager->pad(i), padXManager->pad(i) );
#else
		//m_team[i]->SetPlayer( padXManager->pad(i * 2), padXManager->pad(i * 2 + 1) );

		// 2vs1
		if(i == 0)
			m_team[i]->SetPlayer( padXManager->pad(i * 2), padXManager->pad(i * 2 + 1) );
		else
			m_team[i]->SetPlayer( padXManager->pad(2), padXManager->pad(2) );
#endif
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		// 後でいじる予感
		m_team[i]->SetCommand(&m_command_list[rand()%40 * COMMAND_MAX], &m_command_list[rand()%40 * COMMAND_MAX], 0);
		m_team[i]->SetCommand(&m_command_list[rand()%40 * COMMAND_MAX], &m_command_list[rand()%40 * COMMAND_MAX], 1);
	}

	// テスト用ゲージ(後々削除
	for(int i = 0; i < 10; i++)
	{
		if(i < 5)
		{
			if(!Polygon2D::Create(&m_test_gage[i], device, m_objectList, m_import->texture(GameImport::BLUE_TEX)))
				return false;
		}
		else
		{
			if(!Polygon2D::Create(&m_test_gage[i], device, m_objectList, m_import->texture(GameImport::RED_TEX)))
				return false;
		}
		m_updateList->Link(m_test_gage[i]);
		m_drawListManager->Link(m_test_gage[i], 3, Shader::PAT_2D);
		m_test_gage[i]->pos(540.0f + 20.0f*i, 650.0f, 0.0f);
		m_test_gage[i]->scl(20.0f, 40.0f, 0.0f);
	}

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void CommandManager::Finalize(void)
{
	SafeDeleteArray(m_command_list);

	for(int i = 0; i < _team_max; i++)
	{
		SafeFinalizeDelete(m_team[i]);
	}
}

//=============================================================================
// 更新
//=============================================================================
void CommandManager::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc( "%d\n",m_progress );
#endif

	for(int i = 0; i < _team_max; i++)
	{
		if(m_team[i]->Update())
		{
			//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			// 後でいじる予感
			m_team[i]->SetCommandNext(&m_command_list[rand()%40 * COMMAND_MAX], 0);
			m_team[i]->SetCommandNext(&m_command_list[rand()%40 * COMMAND_MAX], 1);
			if(i == 0)
				m_progress++;
			else
				m_progress--;

			if(m_progress < 0)	// 勝敗処理
				m_game_state = TEAM0_WIN;
			else if(m_progress > 10)
				m_game_state = TEAM1_WIN;

			// テスト用ゲージ（後々削除
			GageUpd();
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CommandManager::Draw(void)
{
	for(int i = 0; i < _team_max; i++)
	{
		m_team[i]->Draw();
	}
}

//=============================================================================
// テスト用ゲージ(後々削除)
//=============================================================================
void CommandManager::GageUpd(void)
{
	for(int i = 0; i < 10; i++)
	{
		if(i < m_progress)
			m_test_gage[i]->texture(m_import->texture(GameImport::BLUE_TEX));
		else
			m_test_gage[i]->texture(m_import->texture(GameImport::RED_TEX));
	}
}

// EOF