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

#include "..\..\..\object\player\player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
GameMaster::GameMaster(AudienceManager *audienceManager,EffectManager *effectManager,Player *redTeam,Player *blueTeam)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_audienceManager = audienceManager;
	m_effectManager   = effectManager;
	m_blueTeam = blueTeam;
	m_redTeam  = redTeam;
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
	//コマンドマネージャ

	//観客制御生成
	AudienceManager *audience;
	if(!AudienceManager::Create( &audience,device,objectList,updateList,drawList,0,ObjectBase::TYPE_3D,
							 "./resources/texture/boy.png",_ScoreMax,D3DXVECTOR3( 100,50,-1000),D3DXVECTOR3(1000,200,-700 ),D3DXVECTOR3(  -1000,50,-1000),D3DXVECTOR3( -100,200,-700 ) ))
		return false;

	//エフェクトマネージャ
	EffectManager *ef;
	if(!EffectManager::Create( &ef,device,objectList,updateList,drawList,5000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) ))
		return false;

	//プレイヤー生成
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,device,objectList,updateList,drawList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/robo.bin");
	Player::Create( &redTeam,device,objectList,updateList,drawList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/robo.bin");

	//ゲームマスター生成
	GameMaster* pointer = new GameMaster( audience,ef,redTeam,blueTeam );
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
	//----------------------------
	// コメント
	//----------------------------
	m_redTeamAddVal = 0;
	m_blueTeamAddVal=0;
	m_totalScore = _ScoreMax;
	m_redTeamScore = _ScoreMax/2;
	m_blueTeamScore=_ScoreMax/2;
	m_redTeam->rot( D3DXVECTOR3(0,3*D3DX_PI /4,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,-3*D3DX_PI/4,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );
	m_redTeam->secondOffsetPos( D3DXVECTOR3(0,0,0));
	m_blueTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );

	m_redTeam->Move( D3DXVECTOR3(-700,0,0),D3DXVECTOR3(-700,0,0),300 );
	m_blueTeam->Move( D3DXVECTOR3(700,0,0),D3DXVECTOR3(700,0,0),300 );

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void GameMaster::Finalize(void)
{

	//プレイヤー削除
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );

	SafeFinalizeDelete( m_audienceManager );
	SafeFinalizeDelete( m_effectManager );
}

//=============================================================================
// 更新
//=============================================================================
void GameMaster::Update(void)
{

	//プレイヤー更新
	m_redTeam->Update();
	m_blueTeam->Update();

	//スコア確定
	DetermineTeamScore();

	//移動値を伝える
	m_audienceManager->MoveBlueTeamFromRedTeam( m_blueTeamAddVal );
	m_audienceManager->MoveRedTeamFromBlueTeam( m_redTeamAddVal );

	//観客更新
	m_audienceManager->Update();
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