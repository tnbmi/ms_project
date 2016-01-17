//*****************************************************************************
//
// リザルトシーン管理 [resultMaster.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "resultMaster.h"
#include "..\..\..\common\safe.h"
#include "..\..\..\common\random\random.h"

#include "..\..\..\manager\manager.h"

#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"
#include "..\..\..\score\score.h"
#include "..\..\..\effectManager\effectManager.h"
#include "..\..\..\object\player\player.h"
#include "..\..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\..\common\complement.h"

#include "..\..\..\view\light\light.h"
#include "..\..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\..\import\result\resultImport.h"

#include "..\..\..\sound\sound.h"
#include "..\..\..\rainManager\rainManager.h"


//=============================================================================
// コンストラクタ
//=============================================================================
ResultMaster::ResultMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,ResultImport *import,FbxTexImport *fbxTexImport,Debugproc *proc,PadXManager* padXMaster,Light *light )
{
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	m_fbxTexImport = fbxTexImport;
	m_light = light;
}

//=============================================================================
// デストラクタ
//=============================================================================
ResultMaster::~ResultMaster(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool ResultMaster::Create(ResultMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						ResultImport* import,FbxTexImport *fbxTexImport,Debugproc* debugproc,PadXManager* padXManager,Light *light)
{

	ResultMaster* pointer = new ResultMaster( device,objectList,updateList,drawList,import,fbxTexImport,debugproc,padXManager,light );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool ResultMaster::Initialize(void)
{
	m_light->dirLightAmbient(0.5,0.5,0.5,1);
	InitializeN();
	return true;
	//スコア生成
	Score *redTeamScore;
	Score *blueTeamScore;
	Score::Create( &redTeamScore,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_2D,m_import );
	Score::Create( &blueTeamScore,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_2D,m_import );

	m_redTeamScore = redTeamScore;
	m_blueTeamScore= blueTeamScore;

	//ねぶた生成
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebta_red.bin",m_fbxTexImport);
	Player::Create( &redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebta_blue.bin",m_fbxTexImport);

	m_redTeam = redTeam;
	m_blueTeam= blueTeam;

	redTeam->StartAnimationChild( 1,60,true );
	blueTeam->StartAnimationChild( 1,60,true );

	redTeam->StartAnimationParent( 1,60,true );
	blueTeam->StartAnimationParent( 1,60,true );

	//おじいちゃん生成
	FbxModel *redGgy;
	FbxModel *blueGgy;
	FbxModel::Create( &redGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/gg_red.bin",m_fbxTexImport );
	FbxModel::Create( &blueGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/gg_blue.bin",m_fbxTexImport );

	//初期化対策
	redGgy->Update();
	blueGgy->Update();
	m_redGgy = redGgy;
	m_blueGgy= blueGgy;

	m_updateList->Link( redGgy );
	m_drawListManager->Link( redGgy,0,Shader::PAT_FBX );
	m_updateList->Link( blueGgy );
	m_drawListManager->Link( blueGgy,0,Shader::PAT_FBX );

	//エフェクトマネージャ生成
	EffectManager *effectManager;
	EffectManager::Create( &effectManager,m_device,m_objectList,m_updateList,m_drawListManager,10000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) );

	m_effectManager = effectManager;

	//polygon2D
	Polygon2D *pol;
	Polygon2D::Create( &pol,m_device,m_objectList,m_import->texture(ResultImport::KEKKA),ObjectBase::TYPE_2D );
	m_updateList->Link( pol );
	m_drawListManager->Link( pol,0,Shader::PAT_2D );

	m_resultPoly = pol;
	m_resultPoly->pos(640,600,0);
	m_resultPoly->scl(300,200,0);
	m_resultPoly->color(1,1,1,1);

	Polygon2D::Create( &pol,m_device,m_objectList,m_import->texture(ResultImport::SYOURIRED),ObjectBase::TYPE_2D );
	m_updateList->Link( pol );
	m_drawListManager->Link( pol,0,Shader::PAT_2D );

	m_winPoly = pol;
	m_winPoly->scl(0,0,0);
	m_winPoly->color(1,1,1,1);
	

	//----------------------------
	//初期位置
	//----------------------------
	m_redTeamScore->pos( D3DXVECTOR3( 640 +320,180,0 ) );
	m_blueTeamScore->pos( D3DXVECTOR3( 320,180,0 ) );
	m_redTeamScore->StartRandView(200);
	m_redTeamScore->score(Manager::scoreRed());
	m_redTeamScore->col(D3DXCOLOR(1,0,0,1));
	
	m_blueTeamScore->StartRandView(200);
	m_blueTeamScore->score(Manager::scoreBlue());
	m_blueTeamScore->col(D3DXCOLOR(0,0,1,1));

	m_redTeam->rot( D3DXVECTOR3(0,PAI,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,PAI,0 ) );
	m_redTeam->Move( D3DXVECTOR3(-500,0,0),D3DXVECTOR3(-500,0,0),300 );
	m_blueTeam->Move( D3DXVECTOR3(500,0,0),D3DXVECTOR3(500,0,0),300 );

	m_redTeam->ApplySclRotPos();
	m_blueTeam->ApplySclRotPos();

	m_redGgy->pos( D3DXVECTOR3(100,0,-1200) );
	m_blueGgy->pos( D3DXVECTOR3(-100,0,-1200) );
	m_redGgy->rot( D3DXVECTOR3(0,PAI,0));
	m_blueGgy->rot( D3DXVECTOR3(0,PAI,0));

	m_effectManager->LoadEffectData( "./resources/effect/FireWorks.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks2.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks3.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/Ene.OEF" );
	m_effectManager->SetOption( InstancingBillboard::OPTION( true,false,false ));

	m_redTeam->StartAnimationSecondChild(1,30,true );
	m_blueTeam->StartAnimationSecondChild(1,30,true );
	m_redGgy->StartAnimation(1,30,true );
	m_blueGgy->StartAnimation(1,30,true );

	m_phase = PHASE_RESULTSTART;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void ResultMaster::Finalize(void)
{
	FinalizeN();
	return;
	SafeFinalizeDelete( m_redTeamScore );
	SafeFinalizeDelete( m_blueTeamScore );
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );
	SafeFinalizeDelete( m_effectManager );
	SafeFinalizeDelete( m_rainManager );
}

//=============================================================================
// 更新
//=============================================================================
void ResultMaster::Update(void)
{
	UpdateN();
	return;

	switch( m_phase )
	{
	case PHASE_RESULTSTART:

		m_redTeamScore->StartRandView( 60 * 3 );
		m_blueTeamScore->StartRandView( 60 * 3 );
		m_phase = PHASE_ANNOUNCEMENT;
		Sound::Play( Sound::SE_DRUM_DODON );
		break;

	case PHASE_ANNOUNCEMENT:

		if( !m_redTeamScore->isRandView() )
		{
			m_light->dirLightAmbient(0.5,0.5,0.5,1);
			m_phase = PHASE_ANNOUNCEFINISH;

			//赤勝ち
			if( m_redTeamScore->score() > m_blueTeamScore->score() )
			{
				m_winPoly->scl( 300,450,1);
				m_winPoly->pos( 1100,500,0);
				m_winPoly->texture( m_import->texture( ResultImport::SYOURIRED ) );
				m_redGgy->StartAnimation(31,90,false );
				m_blueGgy->StartAnimation(91,150,false );
				m_blueTeam->StartAnimationSecondChild( 451,510,false ); 
				m_redTeam->StartAnimationSecondChild( 511,570,false ); 
				Sound::Play( Sound::SE_SYOURI_RED );

			}//青勝ち
			else if( m_redTeamScore->score() < m_blueTeamScore->score() )
			{

				m_winPoly->scl( 300,450,1);
				m_winPoly->pos(200,500,0);
				m_winPoly->texture( m_import->texture( ResultImport::SYOURIBLUE ) );
				m_redGgy->StartAnimation(91,150,false );
				m_blueGgy->StartAnimation(31,90,false );
				m_redTeam->StartAnimationSecondChild( 451,510,false ); 
				m_blueTeam->StartAnimationSecondChild( 511,570,false );
				Sound::Play( Sound::SE_SYOURI_BLUE );
			}//引き分け
			else
			{
				//m_winPoly->scl( 300,450,1);
				//m_winPoly->pos(200,500,0);
				//m_winPoly->texture( m_import->texture( ResultImport::SYOURIBLUE ) );
				m_redGgy->StartAnimation(31,90,false );
				m_blueGgy->StartAnimation(31,90,false );
				m_redTeam->StartAnimationSecondChild( 451,510,false ); 
				m_blueTeam->StartAnimationSecondChild( 451,510,false );
				Sound::Play( Sound::SE_SYOURI_BLUE );
				Sound::Play( Sound::SE_SYOURI_RED );
			}

			Sound::Play( Sound::SE_MORIAGARI );
			m_effectManager->SetEffectGenData(0,140,0,D3DXVECTOR3(-800,1300,600) );
			m_effectManager->SetEffectGenData(1,120,1,D3DXVECTOR3(-0,1000,600) );
			m_effectManager->SetEffectGenData(2,300,2,D3DXVECTOR3(800,1300,600) );
		}


		break;

	case PHASE_ANNOUNCEFINISH:



		break;
	}

	m_redTeamScore->Update();
	m_blueTeamScore->Update();
	m_redTeam->Update();
	m_blueTeam->Update();
	m_effectManager->Update();
	m_rainManager->Update();
}

//=============================================================================
//初期化処理
//=============================================================================

bool ResultMaster::InitializeN()
{
	//エフェクトマネージャ生成
	EffectManager *effectManager;
	EffectManager::Create( &effectManager,m_device,m_objectList,m_updateList,m_drawListManager,10000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) );

	m_effectManager = effectManager;
	m_effectManager->SetOption( InstancingBillboard::OPTION( true,false,false ) );

	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_TeamBlue.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_TeamRed.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_Title1.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_Title2.OEF" );

	//レインマネージャ生成
	RainManager::Create( &m_rainManager,m_device,m_objectList,m_updateList,m_drawListManager,5000,"./resources/texture/sakura.png",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) );
	//m_rainManager->SetRainSeed(1,30,100,200,200,400,D3DXVECTOR3(0.001f,0,0),D3DXVECTOR3(0.002f,0,0),D3DXVECTOR3(-0,200,-300),D3DXVECTOR3(-100,500,300) );


	//スコア生成
	Score *redTeamScore;
	Score *blueTeamScore;
	Score::Create( &redTeamScore,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_2D,m_import );
	Score::Create( &blueTeamScore,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_2D,m_import );

	m_redTeamScore = redTeamScore;
	m_blueTeamScore= blueTeamScore;

	m_redTeamScore->pos( D3DXVECTOR3( 10000+640 +320,180,0 ) );
	m_blueTeamScore->pos( D3DXVECTOR3( 10000+320,180,0 ) );
	m_redTeamScore->StartRandView(200);
	m_redTeamScore->score(Manager::scoreRed());
	m_redTeamScore->col(D3DXCOLOR(1,0,0,1));
	
	m_blueTeamScore->StartRandView(200);
	m_blueTeamScore->score(Manager::scoreBlue());
	m_blueTeamScore->col(D3DXCOLOR(0,0,1,1));

	m_redTeamScore->scl( D3DXVECTOR3( 0,0,0) );
	m_blueTeamScore->scl( D3DXVECTOR3( 0,0,0) );

	//初期化対策
	m_redTeamScore->Update();
	m_blueTeamScore->Update();


	//扇二種用ポリゴン生成
	int rscore,bscore;

	rscore = Manager::scoreRed();
	bscore = Manager::scoreBlue();
	if( rscore > bscore )
	{
		//レフトファン
		Polygon2D::Create( &m_fanLeft,m_device,m_objectList,m_import->texture(ResultImport::REDOUGI),ObjectBase::TYPE_2D );
		m_updateList->Link( m_fanLeft );
		m_drawListManager->Link( m_fanLeft,0,Shader::PAT_2D );

		//ライトファン
		Polygon2D::Create( &m_fanRight,m_device,m_objectList,m_import->texture(ResultImport::REDOUGI),ObjectBase::TYPE_2D );
		m_updateList->Link( m_fanRight );
		m_drawListManager->Link( m_fanRight,0,Shader::PAT_2D );

	}
	else if( bscore > rscore )
	{
		//レフトファン
		Polygon2D::Create( &m_fanLeft,m_device,m_objectList,m_import->texture(ResultImport::BLUEOUGI),ObjectBase::TYPE_2D );
		m_updateList->Link( m_fanLeft );
		m_drawListManager->Link( m_fanLeft,0,Shader::PAT_2D );

		//ライトファン
		Polygon2D::Create( &m_fanRight,m_device,m_objectList,m_import->texture(ResultImport::BLUEOUGI),ObjectBase::TYPE_2D );
		m_updateList->Link( m_fanRight );
		m_drawListManager->Link( m_fanRight,0,Shader::PAT_2D );

	}
	else
	{
		//レフトファン
		Polygon2D::Create( &m_fanLeft,m_device,m_objectList,m_import->texture(ResultImport::BLUEHIKIWAKE),ObjectBase::TYPE_2D );
		m_updateList->Link( m_fanLeft );
		m_drawListManager->Link( m_fanLeft,0,Shader::PAT_2D );

		//ライトファン
		Polygon2D::Create( &m_fanRight,m_device,m_objectList,m_import->texture(ResultImport::REDHIKIWAKE),ObjectBase::TYPE_2D );
		m_updateList->Link( m_fanRight );
		m_drawListManager->Link( m_fanRight,0,Shader::PAT_2D );
	}

	//背景fbxモデル生成
	FbxModel::Create( &m_back,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/Result.bin",m_fbxTexImport );

	m_updateList->Link( m_back );
	m_drawListManager->Link( m_back,0,Shader::PAT_FBX );

	m_redGgy = nullptr;
	m_blueGgy= nullptr;
	m_redNeb = nullptr;
	m_blueNeb= nullptr;

	//ねぶたとじじい生成

	
	if( rscore > bscore )
	{
		FbxModel::Create( &m_redGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_ggy_red.bin",m_fbxTexImport );
		FbxModel::Create( &m_redNeb,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_nebuta_red.bin",m_fbxTexImport );

		m_updateList->Link( m_redGgy );
		m_drawListManager->Link( m_redGgy,0,Shader::PAT_FBX );

		m_updateList->Link( m_redNeb );
		m_drawListManager->Link( m_redNeb,0,Shader::PAT_FBX );
	}
	else if( bscore > rscore )
	{
		FbxModel::Create( &m_blueGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_ggy_blue.bin",m_fbxTexImport );
		FbxModel::Create( &m_blueNeb,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_nebuta_blue.bin",m_fbxTexImport );

		m_updateList->Link( m_blueGgy );
		m_drawListManager->Link( m_blueGgy,0,Shader::PAT_FBX );

		m_updateList->Link( m_blueNeb );
		m_drawListManager->Link( m_blueNeb,0,Shader::PAT_FBX );
	}
	else
	{
		FbxModel::Create( &m_redGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_ggy_red.bin",m_fbxTexImport );
		FbxModel::Create( &m_redNeb,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_nebuta_red.bin",m_fbxTexImport );

		m_updateList->Link( m_redGgy );
		m_drawListManager->Link( m_redGgy,0,Shader::PAT_FBX );

		m_updateList->Link( m_redNeb );
		m_drawListManager->Link( m_redNeb,0,Shader::PAT_FBX );

		FbxModel::Create( &m_blueGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_ggy_blue.bin",m_fbxTexImport );
		FbxModel::Create( &m_blueNeb,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxmodel/result_nebuta_blue.bin",m_fbxTexImport );

		m_updateList->Link( m_blueGgy );
		m_drawListManager->Link( m_blueGgy,0,Shader::PAT_FBX );

		m_updateList->Link( m_blueNeb );
		m_drawListManager->Link( m_blueNeb,0,Shader::PAT_FBX );
	}
	
	//初期位置

	//背景
	m_back->pos( 0,0,0 );
	m_back->rot( D3DXVECTOR3(0,PAI,0 ) );

	//扇
	m_fanLeftRot = -PAI/2;
	m_fanRightRot= PAI/2;

	m_fanLeft->scl(600,600,0);
	m_fanLeft->pos(0,SCREEN_HEIGHT,0 );
	m_fanLeft->rot(0,0,m_fanLeftRot );

	m_fanRight->scl(600,600,0);
	m_fanRight->pos(SCREEN_WIDTH,SCREEN_HEIGHT,0 );
	m_fanRight->rot(0,0,m_fanRightRot);


	//ねぶた
	if( m_redNeb != nullptr )
	{
		m_redNeb->pos( 0,10000,712 );
		m_redNeb->rot(0,PAI,0);
	}

	if( m_blueNeb != nullptr )
	{
		m_blueNeb->pos( 0,10000,712 );
		m_blueNeb->rot(0,PAI,0);
	}


	//じじい
	m_redGgyRot = PAI;
	m_blueGgyRot= PAI;

	if( m_redGgy != nullptr )
	{
		m_redGgy->pos( 0,10000,-82 );
		m_redGgy->rot(0,m_redGgyRot,0);
	}

	if( m_blueGgy != nullptr )
	{
		m_blueGgy->pos(0,10000,-82 );
		m_blueGgy->rot(0,m_blueGgyRot,0);
	}

	m_phase = PHASE_RESULTSTART;
	return true;
}

//=============================================================================
//終了処理
//=============================================================================

void ResultMaster::FinalizeN()
{
	SafeFinalizeDelete( m_effectManager );
	SafeFinalizeDelete( m_rainManager );
	SafeFinalizeDelete( m_redTeamScore );
	SafeFinalizeDelete( m_blueTeamScore );
}

//=============================================================================
//更新処理
//=============================================================================

void ResultMaster::UpdateN()
{
	switch( m_phase )
	{
		case PHASE_RESULTSTART:
			m_redTeamScore->pos( D3DXVECTOR3( 640 +320,100,0 ) );
			m_blueTeamScore->pos( D3DXVECTOR3( 320,100,0 ) );
			m_redTeamScore->scl( D3DXVECTOR3( 50,50,0) );
			m_blueTeamScore->scl( D3DXVECTOR3( 50,50,0) );
			m_redTeamScore->col(D3DXCOLOR(1,0,0,1));
			m_blueTeamScore->col(D3DXCOLOR(0,0,1,1));
			m_redTeamScore->StartRandView( 60 * 3 );
			m_blueTeamScore->StartRandView( 60 * 3 );
			m_phase = PHASE_ANNOUNCEMENT;
			Sound::Play( Sound::SE_DRUM_DODON );
			m_time = 0;
			break;

		case PHASE_ANNOUNCEMENT:

			if( !m_redTeamScore->isRandView() )
			{
				m_phase = PHASE_FANROT;
			}

			break;

		case PHASE_FANROT:
			//扇の回転
			
			m_fanLeftRot += PAI * 0.04f;
			m_fanRightRot+= -PAI * 0.04f;

			if( m_fanLeftRot > PAI/3.5 )
			{
				m_fanLeftRot = PAI/3.5;
				m_fanRightRot= -PAI/3.5;

				m_fanLeft->rot(0,0,m_fanLeftRot);
				m_fanRight->rot(0,0,m_fanRightRot);

				if( m_redGgy != nullptr )
				{
					m_redGgy->StartAnimation(1,90,false );
				}

				if( m_blueGgy != nullptr )
				{
					m_blueGgy->StartAnimation( 1,90,false );
				}

				if( m_redNeb != nullptr )
				{
					m_redNeb->StartAnimation(1,90,false);
				}

				if( m_blueNeb != nullptr )
				{
					m_blueNeb->StartAnimation( 1,90,false );
				}
				
				m_phase = PHASE_MODELIN;
			}

			m_fanLeft->rot(0,0,m_fanLeftRot);
			m_fanRight->rot(0,0,m_fanRightRot);
		break;

		case  PHASE_MODELIN:

			//ねぶたと爺をアニメーションさせながら移動
			if( Manager::scoreRed() > Manager::scoreBlue() )
			{
				WinRedTeam();
			}
			else if( Manager::scoreRed() < Manager::scoreBlue() )
			{
				WinBlueTeam();
			}
			else
			{
				DrawTeam();
			}
			
			//一定位置についたら次のフェーズへ

		break;

		case PHASE_MODELROT:
			m_phase = PHASE_MODELPOSE;
			/*
			m_redGgyRot += -PAI * 0.1f;
			m_blueGgyRot+= PAI * 0.1f;

			if( m_blueGgyRot > PAI )
			{
				m_redGgyRot = PAI;
				m_blueGgyRot = PAI;

				if( m_redGgy != nullptr )
				{
					m_redGgy->rot(0,m_redGgyRot,0);
				}

				if( m_blueGgy != nullptr )
				{
					m_blueGgy->rot(0,m_blueGgyRot,0);
				}

				m_phase = PHASE_MODELPOSE;
			}

			if( m_redGgy != nullptr )
			{
				m_redGgy->rot(0,m_redGgyRot,0);
			}

			if( m_blueGgy != nullptr )
			{
				m_blueGgy->rot(0,m_blueGgyRot,0);
			}
*/
		break;

		case PHASE_MODELPOSE:

			//ねぶたと爺がポーズをしている

			//ポーズ終了で次のフェーズへ
			m_phase = PHASE_FIREWORKSANDRAIN;
		break;

		case PHASE_FIREWORKSANDRAIN:

			//桜吹雪と花火の発射ルーチンセット
			m_rainManager->SetRainSeed(0,80,50,100,150,200,D3DXVECTOR3(2,0,0),D3DXVECTOR3(8,0,0),D3DXVECTOR3(-1200,500,0),D3DXVECTOR3(-1000,1000,0));
			m_rainManager->SetRainSeed(1,80,50,100,100,150,D3DXVECTOR3(-8,0,0),D3DXVECTOR3(-2,0,0),D3DXVECTOR3(1000,500,0),D3DXVECTOR3(1200,1000,0));
			m_rainManager->SetRainSeed(0,80,50,100,150,200,D3DXVECTOR3(2,0,0),D3DXVECTOR3(8,0,0),D3DXVECTOR3(-1200,500,500),D3DXVECTOR3(-1000,1000,500));
			m_rainManager->SetRainSeed(1,80,50,100,100,150,D3DXVECTOR3(-8,0,0),D3DXVECTOR3(-2,0,0),D3DXVECTOR3(1000,500,500),D3DXVECTOR3(1200,1000,500));
			m_effectManager->SetEffectGenData( 0,120,0,D3DXVECTOR3 (-500,1200,1200 ));
			m_effectManager->SetEffectGenData( 1,240,1,D3DXVECTOR3 (500,1400,1200 ));
			m_effectManager->SetEffectGenData( 2,120,2,D3DXVECTOR3 (-750,1500,1500 ));
			m_effectManager->SetEffectGenData( 3,150,3,D3DXVECTOR3 (750,1600,1500 ));
			m_phase = PHASE_FINISH;
		break;

		case PHASE_FINISH:


		break;
	}

	m_effectManager->Update();
	m_rainManager->Update();
	m_redTeamScore->Update();
	m_blueTeamScore->Update();
}

void ResultMaster::WinRedTeam()
{
	m_redNebPos = LerpVec3( D3DXVECTOR3( 0,2000,712 ),D3DXVECTOR3(0,-220,712 ),0,40,m_time,Cube );
	m_redGgyPos = LerpVec3( D3DXVECTOR3( 2000,2000,-82),D3DXVECTOR3(0,100,-82 ),0,40,m_time,Cube);

	m_redNeb->pos( m_redNebPos );
	m_redGgy->pos( m_redGgyPos );

	m_time++;

	if( m_time >= 40 )
	{
		m_phase = PHASE_MODELROT;
	}
}

void ResultMaster::WinBlueTeam()
{
	m_blueNebPos = LerpVec3( D3DXVECTOR3( 0,2000,712 ),D3DXVECTOR3(0,-220,712 ),0,40,m_time,Cube );
	m_blueGgyPos = LerpVec3( D3DXVECTOR3( -2000,2000,-82),D3DXVECTOR3(0,100,-82 ),0,40,m_time,Cube);

	m_blueNeb->pos( m_blueNebPos );
	m_blueGgy->pos( m_blueGgyPos );

	m_time++;

	if( m_time >= 40 )
	{
		m_phase = PHASE_MODELROT;
	}
}

void ResultMaster::DrawTeam()
{
	m_blueNebPos = LerpVec3( D3DXVECTOR3( -334,2000,712 ),D3DXVECTOR3(-334,-220,712 ),0,60,m_time,Cube );
	m_blueGgyPos = LerpVec3( D3DXVECTOR3( -2000,2000,-82),D3DXVECTOR3(-134,100,-82 ),0,60,m_time,Cube);

	m_blueNeb->pos( m_blueNebPos );
	m_blueGgy->pos( m_blueGgyPos );

	m_redNebPos = LerpVec3( D3DXVECTOR3( 334,2000,712 ),D3DXVECTOR3(334,-220,712 ),0,60,m_time,Cube );
	m_redGgyPos = LerpVec3( D3DXVECTOR3( 2000,2000,-82),D3DXVECTOR3(134,100,-82 ),0,60,m_time,Cube);

	m_redNeb->pos( m_redNebPos );
	m_redGgy->pos( m_redGgyPos );

	m_time++;

	if( m_time >= 60 )
	{
		m_phase = PHASE_MODELROT;
	}
}


//=============================================================================
// 描画
//=============================================================================
void ResultMaster::Draw(void)
{
}
