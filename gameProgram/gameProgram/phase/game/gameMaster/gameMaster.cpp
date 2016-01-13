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
#include "..\..\..\common\random\random.h"

#include "..\..\..\manager\manager.h"
#include "..\..\..\import\main\mainImport.h"

#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"

#include "..\..\..\audienceManager\audienceManager.h"
#include "..\..\..\effectManager\effectManager.h"

#include "..\..\..\commandmanager\commandmanager.h"
#include "..\..\..\timemanager\timeManager.h"

#include "..\..\..\object\player\player.h"
#include "..\..\..\ggy2DAnimationManager\ggy2DAnimationManager.h"
#include "..\..\..\import\game\gameImport.h"
#include "..\..\..\common\complement.h"

#include "..\..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\..\sound\sound.h"

#include "..\..\..\tex2DAnimation\tex2DAnimation.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _time_max		= 5460;
const D3DXVECTOR3 _effect_pos[2] = {D3DXVECTOR3(-900.0f, 400.0f, 0.0f),D3DXVECTOR3(900.0f, 400.0f, 0.0f)};

//=============================================================================
// コンストラクタ
//=============================================================================
GameMaster::GameMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,GameImport *import,FbxTexImport *fbxTexImport,Debugproc *proc,PadXManager* padXMaster )
{
	//
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	m_fbxTexImport = fbxTexImport;
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
						GameImport* import,FbxTexImport *fbxTexImport,Debugproc* debugproc,PadXManager* padXManager)
{
	//ゲームマスター生成
	GameMaster* pointer = new GameMaster( device,objectList,updateList,drawList,import,fbxTexImport,debugproc,padXManager );
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
							 "./resources/texture/boy.png",_ScoreMax,D3DXVECTOR3( 100.0f,50.0f,-1500.0f),D3DXVECTOR3(1500.0f,200.0f,-450.0f ),D3DXVECTOR3(  -1500.0f,50.0f,-1500.0f),D3DXVECTOR3( -100.0f,200.0f,-450.0f ) ))
		return false;

	//エフェクトマネージャ
	EffectManager *ef;
	if(!EffectManager::Create( &ef,m_device,m_objectList,m_updateList,m_drawListManager,5000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) ))
		return false;

	//プレイヤー生成
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebta_blue_short.bin",m_fbxTexImport);
	Player::Create( &redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebta_red_short.bin",m_fbxTexImport);

	//じじいポリゴン召喚
	Ggy2DAnimationManager::Create( &m_blueGgyAnim,m_device,m_objectList,m_updateList,m_drawListManager );
	Ggy2DAnimationManager::Create( &m_redGgyAnim,m_device,m_objectList,m_updateList,m_drawListManager );

	//カウントダウン　コール系ポリゴン（使いまわし)
	Polygon2D::Create( &m_call,m_device,m_objectList,nullptr,ObjectBase::TYPE_2D );
	m_updateList->Link( m_call );
	m_drawListManager->Link( m_call,0,Shader::PAT_2D );

	m_call->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0);
	m_call->scl( 0,0,0 );

	//カウントダウンアニメーション制御
	Tex2DAnimation::Create( &m_countDown );
	m_countDown->Set2DPolygon( m_call );
	m_countDown->SetTexture( m_import->texture( GameImport::COUNTDOWN ) );
	m_countDown->SetAnimationData(60,3,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.2f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.2f,1.0f),D3DXVECTOR2(0.2f,0.0f) );
	m_countDown->StartAnimation(false);
	m_countDown->SetTextureToPolygon();
	MainImport* mainImport = Manager::mainImport();
	m_blueGgyAnim->SetTexture(0,mainImport->texture( MainImport::GGYBLUE_WAIT ) );
	m_blueGgyAnim->SetTexture(1,mainImport->texture( MainImport::GGYBLUE_WIN ) );
	m_blueGgyAnim->SetTexture(2,mainImport->texture( MainImport::GGYBLUE_LOSE ) );
	m_blueGgyAnim->SetTexture(3,mainImport->texture( MainImport::GGYBLUE_UP ) );
	m_blueGgyAnim->SetTexture(4,mainImport->texture( MainImport::GGYBLUE_RIGHT ) );
	m_blueGgyAnim->SetTexture(5,mainImport->texture( MainImport::GGYBLUE_DOWN ) );
	m_blueGgyAnim->SetTexture(6,mainImport->texture( MainImport::GGYBLUE_LEFT ) );
	m_blueGgyAnim->SetTexture(7,mainImport->texture( MainImport::GGYBLUE_P ) );
	m_blueGgyAnim->SetTexture(8,mainImport->texture( MainImport::GGYBLUE_POSE ) );

	m_blueGgyAnim->pos(D3DXVECTOR3( 280,580,0 ));
	m_blueGgyAnim->scl(D3DXVECTOR3( 320,256,0 ));
	m_blueGgyAnim->StartAnimation(0,true);
	m_blueGgyAnim->Update();

	m_redGgyAnim->SetTexture(0,mainImport->texture( MainImport::GGYRED_WAIT ) );
	m_redGgyAnim->SetTexture(1,mainImport->texture( MainImport::GGYRED_WIN ) );
	m_redGgyAnim->SetTexture(2,mainImport->texture( MainImport::GGYRED_LOSE ) );
	m_redGgyAnim->SetTexture(3,mainImport->texture( MainImport::GGYRED_UP ) );
	m_redGgyAnim->SetTexture(4,mainImport->texture( MainImport::GGYRED_RIGHT ) );
	m_redGgyAnim->SetTexture(5,mainImport->texture( MainImport::GGYRED_DOWN ) );
	m_redGgyAnim->SetTexture(6,mainImport->texture( MainImport::GGYRED_LEFT ) );
	m_redGgyAnim->SetTexture(7,mainImport->texture( MainImport::GGYRED_P ) );
	m_redGgyAnim->SetTexture(8,mainImport->texture( MainImport::GGYRED_POSE ) );

	m_redGgyAnim->pos(D3DXVECTOR3( 1000,580,0 ));
	m_redGgyAnim->scl(D3DXVECTOR3( 320,256,0 ));
	m_redGgyAnim->StartAnimation(0,true);
	m_redGgyAnim->Update();


	redTeam->StartAnimationSecondChild( 1,570,true );
	blueTeam->StartAnimationSecondChild( 1,570,true );

	redTeam->StartAnimationChild( 1,60,true );
	blueTeam->StartAnimationChild( 1,60,true );

	redTeam->StartAnimationParent( 1,60,true );
	blueTeam->StartAnimationParent( 1,60,true );

	m_audienceManager = audience;
	m_effectManager   = ef;
	m_blueTeam = blueTeam;
	m_redTeam  = redTeam;

	//----------------------------
	//カットイン
	//----------------------------
	Polygon2D::Create( &m_redTeamCutIn.pol,m_device,m_objectList,m_import->texture(GameImport::CUTINRED),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueTeamCutIn.pol,m_device,m_objectList,m_import->texture(GameImport::CUTINBLUE),ObjectBase::TYPE_2D );

	m_updateList->Link( m_redTeamCutIn.pol );
	m_drawListManager->Link( m_redTeamCutIn.pol,0,Shader::PAT_2D );
	m_updateList->Link( m_blueTeamCutIn.pol );
	m_drawListManager->Link( m_blueTeamCutIn.pol,0,Shader::PAT_2D );

	m_redTeamCutIn.pol->pos(0.0f,1500.0f,0.0f);
	m_redTeamCutIn.pol->scl(450.0f,300.0f,0.0f);
	m_redTeamCutIn.addVal = 0;
	m_redTeamCutIn.time = 0;
	m_redTeamCutIn.stPos = D3DXVECTOR3( SCREEN_WIDTH + 200.0f,500.0f,0.0f );
	m_redTeamCutIn.edPos = D3DXVECTOR3( SCREEN_WIDTH - 420.0f,500.0f,0.0f );
	m_redTeamCutIn.isCutIn = false;

	m_blueTeamCutIn.pol->pos(0.0f,1500.0f,0.0f);
	m_blueTeamCutIn.pol->scl(450.0f,300.0f,0.0f);
	m_blueTeamCutIn.addVal = 0;
	m_blueTeamCutIn.time = 0;
	m_blueTeamCutIn.stPos = D3DXVECTOR3( -200.0f,500.0f,0.0f );
	m_blueTeamCutIn.edPos = D3DXVECTOR3( 420.0f,500.0f,0.0f );
	m_blueTeamCutIn.isCutIn = false;

	m_redTeamCutIn.stBufPos = m_redTeamCutIn.stPos;
	m_redTeamCutIn.edBufPos = m_redTeamCutIn.edPos;
	m_blueTeamCutIn.stBufPos = m_blueTeamCutIn.stPos;
	m_blueTeamCutIn.edBufPos = m_blueTeamCutIn.edPos;

	//----------------------------
	// 配置
	//----------------------------
	m_redTeamAddVal = 0;
	m_blueTeamAddVal=0;
	m_totalScore = _ScoreMax;
	m_redTeamScore = _ScoreMax/2;
	m_blueTeamScore=_ScoreMax/2;
	m_redTeam->rot( D3DXVECTOR3(0,-3*PAI*0.25f,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,3*PAI*0.25f,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );
	m_redTeam->secondOffsetPos( D3DXVECTOR3(0,0,0));
	m_blueTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );

	m_redTeam->Move( D3DXVECTOR3(750.0f,0.0f,0.0f),D3DXVECTOR3(750.0f,0.0f,0.0f),300.0f );
	m_blueTeam->Move( D3DXVECTOR3(-750.0f,0.0f,0.0f),D3DXVECTOR3(-750.0f,0.0f,0.0f),300.0f );
	m_redTeam->ApplySclRotPos();
	m_blueTeam->ApplySclRotPos();

	m_effectManager->LoadEffectData( "./resources/effect/FireWorks.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks2.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks3.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/Ene.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/Effect.OEF" );
	m_effectManager->SetOption( InstancingBillboard::OPTION( true,false,false ));

	//アニメーションデータいれてやる
	m_nebAnim[NANIM_WAIT].stFrame = 1;
	m_nebAnim[NANIM_WAIT].edFrame = 30;
	m_nebAnim[NANIM_WAIT].polyGgyAnimIdx =0;

	m_nebAnim[NANIM_ACOMUP].stFrame = 31;
	m_nebAnim[NANIM_ACOMUP].edFrame = 45;
	m_nebAnim[NANIM_ACOMUP].polyGgyAnimIdx = 3;

	m_nebAnim[NANIM_BCOMUP].stFrame = 46;
	m_nebAnim[NANIM_BCOMUP].edFrame = 60;
	m_nebAnim[NANIM_BCOMUP].polyGgyAnimIdx = 3;

	m_nebAnim[NANIM_ACOMR].stFrame = 61;
	m_nebAnim[NANIM_ACOMR].edFrame = 75;
	m_nebAnim[NANIM_ACOMR].polyGgyAnimIdx = 4;

	m_nebAnim[NANIM_BCOMR].stFrame = 76;
	m_nebAnim[NANIM_BCOMR].edFrame = 90;
	m_nebAnim[NANIM_BCOMR].polyGgyAnimIdx = 4;

	m_nebAnim[NANIM_ACOMDOWN].stFrame = 91;
	m_nebAnim[NANIM_ACOMDOWN].edFrame = 105;
	m_nebAnim[NANIM_ACOMDOWN].polyGgyAnimIdx = 5;

	m_nebAnim[NANIM_BCOMDOWN].stFrame = 106;
	m_nebAnim[NANIM_BCOMDOWN].edFrame = 120;
	m_nebAnim[NANIM_BCOMDOWN].polyGgyAnimIdx = 5;

	m_nebAnim[NANIM_ACOML].stFrame = 121;
	m_nebAnim[NANIM_ACOML].edFrame = 135;
	m_nebAnim[NANIM_ACOML].polyGgyAnimIdx = 6;

	m_nebAnim[NANIM_BCOML].stFrame = 136;
	m_nebAnim[NANIM_BCOML].edFrame = 150;
	m_nebAnim[NANIM_BCOML].polyGgyAnimIdx = 6;

	m_nebAnim[NANIM_SAME1].stFrame = 151;
	m_nebAnim[NANIM_SAME1].edFrame = 210;
	m_nebAnim[NANIM_SAME1].polyGgyAnimIdx = 8;

	m_nebAnim[NANIM_SAME2].stFrame = 211;
	m_nebAnim[NANIM_SAME2].edFrame = 270;
	m_nebAnim[NANIM_SAME2].polyGgyAnimIdx = 8;

	m_nebAnim[NANIM_SAME3].stFrame = 271;
	m_nebAnim[NANIM_SAME3].edFrame = 330;
	m_nebAnim[NANIM_SAME3].polyGgyAnimIdx = 8;

	m_nebAnim[NANIM_WIN].stFrame = 331;
	m_nebAnim[NANIM_WIN].edFrame = 390;
	m_nebAnim[NANIM_WIN].polyGgyAnimIdx = 1;

	m_nebAnim[NANIM_LOSE].stFrame = 391;
	m_nebAnim[NANIM_LOSE].edFrame = 450;
	m_nebAnim[NANIM_LOSE].polyGgyAnimIdx = 2;

	redTeam->StartAnimationSecondChild( m_nebAnim[NANIM_WAIT].stFrame,m_nebAnim[NANIM_WAIT].edFrame,true );
	m_redGgyAnim->StartAnimation( m_nebAnim[NANIM_WAIT].polyGgyAnimIdx,true );
	blueTeam->StartAnimationSecondChild( m_nebAnim[NANIM_WAIT].stFrame,m_nebAnim[NANIM_WAIT].edFrame,true );
	m_blueGgyAnim->StartAnimation( m_nebAnim[NANIM_WAIT].polyGgyAnimIdx,true );

	//ゲームフェーズ設定
	m_gamePhase = PHASE_COUNTDOWN;

	//debug
	//m_gamePhase = PHASE_GAME;

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

	SafeFinalizeDelete( m_blueGgyAnim );
	SafeFinalizeDelete( m_redGgyAnim );

	SafeFinalizeDelete( m_countDown );
}

//=============================================================================
// 更新
//=============================================================================
bool GameMaster::Update(void)
{
	switch( m_gamePhase )
	{
		case PHASE_COUNTDOWN:

			UpdateCountDown();

			break;

		case PHASE_CALL:

			UpdateCall();

			break;

		case PHASE_GAME:

			UpdateGame();

			break;

		case PHASE_FINISH:

			UpdateFinish();

			break;

		case PHASE_NEXT_SCENE:

			return UpdateNextScene();
			break;
	}

	m_blueGgyAnim->Update();
	m_redGgyAnim->Update();
	UpdateCutIn();

	return false;
}

//=============================================================================
//スコア加算
//=============================================================================

void GameMaster::AddTeamScore( const int addRedTeamVal,const int addBlueTeamVal )
{
	int addRVal = addRedTeamVal - addBlueTeamVal;
	int addBVal = addBlueTeamVal- addRedTeamVal;

	m_redTeamAddVal += addRVal;
	m_blueTeamAddVal+= addBVal;
}

//=============================================================================
//スコアを確定する
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

//=============================================================================
//アニメーション選択
//=============================================================================

void GameMaster::SelectAnimation( const int judge,Player *player,Ggy2DAnimationManager *ggy,CUTIN *cutIn,Sound::SOUND_TABLE *soundTable )
{
	//	ここでランダムで違うポーズをだす
	int r = Random::Rand() %2;
	int s = Random::Rand( 0,2 );

	switch( judge )
	{
		case 0:

			player->StartAnimationSecondChild( m_nebAnim[ NANIM_ACOMUP+r ].stFrame,m_nebAnim[ NANIM_ACOMUP+r ].edFrame,false );
			ggy->StartAnimation(m_nebAnim[ NANIM_ACOMUP ].polyGgyAnimIdx,false );
			Sound::Play( soundTable[0] );
			break;
		case 1:

			player->StartAnimationSecondChild( m_nebAnim[ NANIM_ACOMDOWN +r].stFrame,m_nebAnim[ NANIM_ACOMDOWN +r].edFrame,false );
			ggy->StartAnimation(m_nebAnim[ NANIM_ACOMDOWN +r].polyGgyAnimIdx,false );
			Sound::Play( soundTable[1] );
			break;

		case 2:

			player->StartAnimationSecondChild( m_nebAnim[ NANIM_ACOML+r ].stFrame,m_nebAnim[ NANIM_ACOML+r ].edFrame,false );
			ggy->StartAnimation(m_nebAnim[ NANIM_ACOML+r ].polyGgyAnimIdx,false );
			Sound::Play( soundTable[0] );
			break;

		case 3:

			player->StartAnimationSecondChild( m_nebAnim[ NANIM_ACOMR +r].stFrame,m_nebAnim[ NANIM_ACOMR +r].edFrame,false );
			ggy->StartAnimation(m_nebAnim[ NANIM_ACOMR+r ].polyGgyAnimIdx,false );
			Sound::Play( soundTable[1] );
			break;

		case 4:

			player->StartAnimationSecondChild( m_nebAnim[ NANIM_SAME1+s ].stFrame,m_nebAnim[ NANIM_SAME1 +s].edFrame,false );
			ggy->StartAnimation(m_nebAnim[ NANIM_SAME1+s ].polyGgyAnimIdx,false );
			m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(0,900,400));

			//汎用同時押し音
			Sound::Play( Sound::SE_SAME );
			//専用同時押し音
			Sound::Play( soundTable[2] );
			//専用同時押し音ねぶた
			Sound::Play( soundTable[4] );

			Sound::Play( Sound::SE_FIREWORKS );
			if( !cutIn->isCutIn )
			{
				cutIn->time = 0;
				cutIn->addVal =1;
				cutIn->stBufPos = cutIn->stPos;
				cutIn->edBufPos = cutIn->edPos;
				cutIn->isCutIn = true;
			}
			break;

		case 5:
			//m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(0,900,1000));
			//Sound::Play( Sound::SE_FIREWORKS );
			player->StartAnimationSecondChild( m_nebAnim[ NANIM_LOSE ].stFrame,m_nebAnim[ NANIM_LOSE ].edFrame,false );
			ggy->StartAnimation(m_nebAnim[ NANIM_LOSE ].polyGgyAnimIdx,false );
			Sound::Play( soundTable[3] );
			/*
			if( !cutIn->isCutIn )
			{
				cutIn->time = 0;
				cutIn->addVal =1;
				cutIn->stBufPos = cutIn->stPos;
				cutIn->edBufPos = cutIn->edPos;
				cutIn->isCutIn = true;
			}
			*/
			break;

		case 6:


			break;

	}
}

//=============================================================================
//カットイン更新
//=============================================================================
void GameMaster::UpdateCutIn()
{
	D3DXVECTOR3 vec;
	int time = m_blueTeamCutIn.time % _cutInFrame;

	m_blueTeamCutIn.time+= m_blueTeamCutIn.addVal;

	if( m_blueTeamCutIn.time == (_cutInFrame + 1) )
	{
		m_blueTeamCutIn.stBufPos = m_blueTeamCutIn.edPos;
		m_blueTeamCutIn.edBufPos = m_blueTeamCutIn.edPos;
	}

	if( m_blueTeamCutIn.time == (_cutInFrame*4 ) )
	{
		m_blueTeamCutIn.stBufPos = m_blueTeamCutIn.edPos;
		m_blueTeamCutIn.edBufPos = m_blueTeamCutIn.stPos;		
	}

	if( m_blueTeamCutIn.time > (_cutInFrame*5) )
	{
		m_blueTeamCutIn.addVal = 0;
		m_blueTeamCutIn.stBufPos = m_blueTeamCutIn.stPos;
		m_blueTeamCutIn.edBufPos = m_blueTeamCutIn.edPos;
		m_blueTeamCutIn.time = 0;
		m_blueTeamCutIn.isCutIn = false;
	}

	vec = LerpVec3( m_blueTeamCutIn.stBufPos,m_blueTeamCutIn.edBufPos,0.0f,(float)_cutInFrame,(float)time,Cube );

	m_blueTeamCutIn.pol->pos( vec );

	time = m_redTeamCutIn.time % _cutInFrame;
	
	m_redTeamCutIn.time+= m_redTeamCutIn.addVal;

	if( m_redTeamCutIn.time == (_cutInFrame + 1) )
	{	
		m_redTeamCutIn.stBufPos = m_redTeamCutIn.edPos;
		m_redTeamCutIn.edBufPos = m_redTeamCutIn.edPos;
	}	
	if( m_redTeamCutIn.time == (_cutInFrame*4 ) )
	{	
		m_redTeamCutIn.stBufPos = m_redTeamCutIn.edPos;
		m_redTeamCutIn.edBufPos = m_redTeamCutIn.stPos;		
	}	
		
	if( m_redTeamCutIn.time > (_cutInFrame*5) )
	{	
		m_redTeamCutIn.addVal = 0;
		m_redTeamCutIn.stBufPos = m_redTeamCutIn.stPos;
		m_redTeamCutIn.edBufPos = m_redTeamCutIn.edPos;
		m_redTeamCutIn.time = 0;
		m_redTeamCutIn.isCutIn = false;
	}

	vec = LerpVec3( m_redTeamCutIn.stBufPos,m_redTeamCutIn.edBufPos,0.0f,(float)_cutInFrame,(float)time,Cube );

	m_redTeamCutIn.pol->pos( vec );

}

//=============================================================================
//カウントダウン更新
//=============================================================================
void GameMaster::UpdateCountDown()
{
	m_call->scl( 400,200,0 );
	m_countDown->Update();
	if( m_countDown->isEndAnimation() )
	{
		m_gamePhase = PHASE_CALL;
		m_call->texcoord(0,0.6f,0.0f);
		m_call->texcoord(1,0.8f,0.0f);
		m_call->texcoord(2,0.6f,1.0f);
		m_call->texcoord(3,0.8f,1.0f);
		m_callTime = 0.0f;

		Sound::Play(Sound::SE_START_VOICE);
	}
}

//=============================================================================
//コール更新
//=============================================================================
void GameMaster::UpdateCall()
{
	D3DXVECTOR3 scl;

	scl = LerpVec3( D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0),D3DXVECTOR3(400,200,0),0,60,m_callTime,Cube );
	m_callTime++;
	m_call->scl(scl);

	if( m_callTime > 60 )
	{
		m_gamePhase = PHASE_GAME;
		m_call->scl(0,0,0);
	}
}

//=============================================================================
//ゲーム更新
//=============================================================================
void GameMaster::UpdateGame()
{
// エフェクトマネージャ
	m_effectManager->Update();

	//プレイヤー更新
	m_redTeam->Update();
	m_blueTeam->Update();

	// コマンドマネージャ
	CommandManager::COM_MANA_RTN get = {0,0};
	get = m_command_manager->Update();
	AddTeamScore(get.score[1], get.score[0]);

	Sound::SOUND_TABLE blueTable[5]={ Sound::SE_ATTACK_BLUE1,Sound::SE_ATTACK_BLUE2,Sound::SE_ATTACK_BLUE3,Sound::SE_FAIL_BLUE,Sound::SE_SAME_NEB_BLUE };
	Sound::SOUND_TABLE redTable[5] ={ Sound::SE_ATTACK_RED1,Sound::SE_ATTACK_RED2,Sound::SE_ATTACK_RED3,Sound::SE_FAIL_RED,Sound::SE_SAME_NEB_RED };;

	SelectAnimation( get.state[1],m_redTeam,m_redGgyAnim,&m_redTeamCutIn,redTable );
	SelectAnimation( get.state[0],m_blueTeam,m_blueGgyAnim,&m_blueTeamCutIn,blueTable );

	//スコア確定
	DetermineTeamScore();

	//移動値を伝える
	m_audienceManager->MoveBlueTeamFromRedTeam( m_blueTeamAddVal );
	m_audienceManager->MoveRedTeamFromBlueTeam( m_redTeamAddVal );

	//観客更新
	m_audienceManager->Update();

	// タイムマネージャ
	if(m_time_manager->Update())
	{
		m_gamePhase = PHASE_FINISH;

		m_call->texcoord(0,0.8f,0);
		m_call->texcoord(1,1.0f,0);
		m_call->texcoord(2,0.8f,1);
		m_call->texcoord(3,1.0f,1);
		m_callTime = 0;
		Sound::Play(Sound::SE_END_VOICE);
	}

	m_blueTeamAddVal = 0;
	m_redTeamAddVal	 = 0;
}

//=============================================================================
//終了更新
//=============================================================================
void GameMaster::UpdateFinish()
{
	D3DXVECTOR3 scl;

	scl = LerpVec3( D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0),D3DXVECTOR3(400,200,0),0,60,m_callTime,Cube );
	m_callTime++;
	m_call->scl(scl);

	if( m_callTime > 60 )
	{
		m_gamePhase = PHASE_NEXT_SCENE;
		m_callTime = 0;
	}
}
//=============================================================================
//移行更新
//=============================================================================
bool GameMaster::UpdateNextScene()
{
	m_callTime++;

	if( m_callTime == 60 )
	{
		return true;
	}

	return false;
}

//=============================================================================
//カットイン更新
//=============================================================================

// EOF