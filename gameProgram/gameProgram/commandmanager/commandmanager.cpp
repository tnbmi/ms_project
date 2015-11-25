//*****************************************************************************
//
// コマンドチェック処理マネージャ [commandmanager.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandmanager.h"
#include "..\common\safe.h"

#include "..\input\padX\padXManager.h"

#include "..\debugproc\debugproc.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\import\game\gameImport.h"

#include "..\commandmanager\commandteam\commandteam.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _team_max = 2;
const int _progress_neutral = 5;
const int _progress_team0_lead = 3;
const int _progress_team1_lead = 7;
const int _list_command_max = 6;
const int _list_pattern_max = 40;
const D3DXVECTOR3 _team_position[2] = {D3DXVECTOR3(84.0f, 444.0f, 0.0f),D3DXVECTOR3(1124.0f, 444.0f, 0.0f)};

//=============================================================================
// コンストラクタ
//=============================================================================
Commandmanager::Commandmanager(void)
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
Commandmanager::~Commandmanager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Commandmanager::Create(Commandmanager** outPointer,
							PadXManager* padXManager,
							Debugproc* debugproc,
							ObjectList* objList,
							UpdateList* updList,
							DrawListManager* drwList,
							LPDIRECT3DDEVICE9 device,
							GameImport* import)
{
	Commandmanager* pointer = new Commandmanager();
	if(!pointer->Initialize(padXManager, debugproc, objList, updList, drwList, device, import))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Commandmanager::Initialize(PadXManager* padXManager,
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

	m_command_list = new int [_list_pattern_max * _list_command_max];

	//---------------------------------------------------------------------------------------------
	// 後でデータロードに置き換えるー
	//---------------------------------------------------------------------------------------------
	m_command_list[0] = XINPUT_GAMEPAD_DPAD_UP;
	m_command_list[1] = XINPUT_GAMEPAD_Y;
	m_command_list[2] = XINPUT_GAMEPAD_DPAD_RIGHT;
	m_command_list[3] = XINPUT_GAMEPAD_DPAD_DOWN;
	m_command_list[4] = XINPUT_GAMEPAD_A;
	m_command_list[5] = XINPUT_GAMEPAD_X;

	for(int i = 0; i < _team_max; i++)
	{
		Commandteam::Create(&m_team[i], m_objectList, m_updateList, m_drawListManager, device, import, _team_position[i]);
		m_team[i]->debugproc(debugproc);
#ifdef _DEBUG
		m_team[i]->SetPlayer( padXManager->pad(i), padXManager->pad(i) );
#else
		m_team[i]->SetPlayer( padXManager->pad(i * 2), padXManager->pad(i * 2 + 1) );
#endif
		m_team[i]->SetCommand(&m_command_list[0 * _list_command_max]);
	}

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Commandmanager::Finalize(void)
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
void Commandmanager::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc( "%d\n",m_progress );
#endif

	for(int i = 0; i < _team_max; i++)
	{
		if(m_team[i]->Update())
		{
			m_team[i]->SetCommand(&m_command_list[0 * _list_command_max]);
			if(i == 0)
				m_progress--;
			else
				m_progress++;
		}
	}

	if(m_progress > _progress_team0_lead && m_progress < _progress_team1_lead)
	{
		m_team[0]->SetFragLose(false);
		m_team[1]->SetFragLose(false);
	}
	else if(m_progress >= _progress_team1_lead)
	{
		m_team[0]->SetFragLose(true);
		m_team[1]->SetFragLose(false);
	}
	else if(m_progress <= _progress_team0_lead)
	{
		m_team[0]->SetFragLose(false);
		m_team[1]->SetFragLose(true);
	}
}

//=============================================================================
// 描画
//=============================================================================
void Commandmanager::Draw(void)
{
	for(int i = 0; i < _team_max; i++)
	{
		m_team[i]->Draw();
	}
}

// EOF