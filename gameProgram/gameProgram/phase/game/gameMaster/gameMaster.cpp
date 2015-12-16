//*****************************************************************************
//
// ゲームマスター [GameMaster.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "GameMaster.h"
#include "..\..\..\common\safe.h"

#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"

#include "..\..\..\audienceManager\audienceManager.h"
#include "..\..\..\effectManager\effectManager.h"

#include "..\..\..\commandmanager\commandmanager.h"
#include "..\..\..\timemanager\timeManager.h"

#include "..\..\..\object\player\player.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _time_max		= 3000;

//=============================================================================
// コンストラクタ
//=============================================================================
GameMaster::GameMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,GameImport *import,Debugproc *proc,PadXManager* padXMaster )
{
	//
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	m_command_manager	= nullptr;
	m_time_manager		= nullptr;
	//----------------------------
	// メンバー初期化
	//----------------------------
}

//=============================================================================
// デストラクタ
//=============================================================================
GameMaster::~GameMaster(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool GameMaster::Create(GameMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						GameImport* import,Debugproc* debugproc,PadXManager* padXManager)
{
	//ゲームマスター生成
	GameMaster* pointer = new GameMaster( device,objectList,updateList,drawList,import,debugproc,padXManager );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool GameMaster::Initialize(void)
{

	//コマンドマネージャ
	CommandManager::Create(&m_command_manager, m_padXManager, m_debugProc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);
	// タイムマネージャ生成
	TimeManager::Create(&m_time_manager, m_objectList, m_updateList, m_drawListManager, m_device, m_import, _time_max);

	//観客制御生成
	AudienceManager *audience;
	if(!AudienceManager::Create( &audience,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,
							 "./resources/texture/boy.png",_ScoreMax,D3DXVECTOR3( 100.0f,50.0f,-1000.0f),D3DXVECTOR3(1000.0f,200.0f,-700.0f ),D3DXVECTOR3(  -1000.0f,50.0f,-1000.0f),D3DXVECTOR3( -100.0f,200.0f,-700.0f ) ))
		return false;

	//エフェクトマネージャ
	EffectManager *ef;
	if(!EffectManager::Create( &ef,m_device,m_objectList,m_updateList,m_drawListManager,5000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) ))
		return false;

	//プレイヤー生成
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebblue.bin");
	Player::Create( &redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebred.bin");

	redTeam->StartAnimationSecondChild( 1,390,true );
	blueTeam->StartAnimationSecondChild( 1,390,true );

	m_audienceManager = audience;
	m_effectManager   = ef;
	m_blueTeam = blueTeam;
	m_redTeam  = redTeam;

	//----------------------------
	// 配置
	//----------------------------
	m_redTeamAddVal = 0;
	m_blueTeamAddVal=0;
	m_totalScore = _ScoreMax;
	m_redTeamScore = _ScoreMax/2;
	m_blueTeamScore=_ScoreMax/2;
	m_redTeam->rot( D3DXVECTOR3(0,-3*D3DX_PI /4,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,3*D3DX_PI/4,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );
	m_redTeam->secondOffsetPos( D3DXVECTOR3(0,0,0));
	m_blueTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );

	m_redTeam->Move( D3DXVECTOR3(700.0f,0.0f,0.0f),D3DXVECTOR3(700.0f,0.0f,0.0f),300.0f );
	m_blueTeam->Move( D3DXVECTOR3(-700.0f,0.0f,0.0f),D3DXVECTOR3(-700.0f,0.0f,0.0f),300.0f );

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void GameMaster::Finalize(void)
{
	// コマンド
	SafeFinalizeDelete(m_command_manager);
	// タイム
	SafeFinalizeDelete(m_time_manager);

	//プレイヤー削除
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );

	SafeFinalizeDelete( m_audienceManager );
	SafeFinalizeDelete( m_effectManager );
}

//=============================================================================
// 更新
//=============================================================================
bool GameMaster::Update(void)
{

	//プレイヤー更新
	m_redTeam->Update();
	m_blueTeam->Update();

	// コマンドマネージャ
	CommandManager::COM_MANA_RTN get = {0,0};
	get = m_command_manager->Update();
	AddTeamScore(get.score[0], get.score[1]);

	//スコア確定
	DetermineTeamScore();

	//移動値を伝える
	m_audienceManager->MoveBlueTeamFromRedTeam( m_blueTeamAddVal );
	m_audienceManager->MoveRedTeamFromBlueTeam( m_redTeamAddVal );

	//観客更新
	m_audienceManager->Update();

	// タイムマネージャ
	if(m_time_manager->Update())
		return true;

	m_blueTeamAddVal = 0;
	m_redTeamAddVal	 = 0;

	return false;
}

//=============================================================================
//
//=============================================================================

void GameMaster::AddTeamScore( const int addRedTeamVal,const int addBlueTeamVal )
{
	int addRVal = addRedTeamVal - addBlueTeamVal;
	int addBVal = addBlueTeamVal- addRedTeamVal;

	m_redTeamAddVal += addRVal;
	m_blueTeamAddVal+= addBVal;
}

//=============================================================================
//
//=============================================================================

void GameMaster::DetermineTeamScore()
{
	int rOffset = 0;
	int bOffset = 0;
	int rScoreBuff = m_redTeamScore + m_redTeamAddVal;
	int bScoreBuff = m_blueTeamScore+ m_blueTeamAddVal;

	//各チームがスコア超えてないかチェック
	if( rScoreBuff < 0 )
	{
		rOffset = rScoreBuff;
	}
	else if( rScoreBuff > m_totalScore )
	{
		rOffset = rScoreBuff - m_totalScore;
	}

	if( bScoreBuff < 0 )
	{
		bOffset = bScoreBuff;
	}
	else if( bScoreBuff > m_totalScore )
	{
		bOffset = bScoreBuff - m_totalScore;
	}

	m_redTeamAddVal -= rOffset;
	m_blueTeamAddVal-= bOffset;

	m_redTeamScore += m_redTeamAddVal;
	m_blueTeamScore += m_blueTeamAddVal;
}


// EOF