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

	RainManager::Create( &m_rainManager,m_device,m_objectList,m_updateList,m_drawListManager,5000,"./resources/texture/sakura.png",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) );

	m_rainManager->SetRainSeed(0,30,100,200,200,400,D3DXVECTOR3(0.001f,0,0),D3DXVECTOR3(0.002f,0,0),D3DXVECTOR3(-2800,1500,-300),D3DXVECTOR3(-2850,2000,300) );

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void ResultMaster::Finalize(void)
{
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

			}
			else
			{

				m_winPoly->scl( 300,450,1);
				m_winPoly->pos(200,500,0);
				m_winPoly->texture( m_import->texture( ResultImport::SYOURIBLUE ) );
				m_redGgy->StartAnimation(91,150,false );
				m_blueGgy->StartAnimation(31,90,false );
				m_redTeam->StartAnimationSecondChild( 451,510,false ); 
				m_blueTeam->StartAnimationSecondChild( 511,570,false );
				Sound::Play( Sound::SE_SYOURI_BLUE );
			}

		}

		Sound::Play( Sound::SE_MORIAGARI );

		m_fireTime = _shotFrame;
		break;

	case PHASE_ANNOUNCEFINISH:

		if( m_fireTime >= _shotFrame )
		{
			D3DXVECTOR3 pos;
			pos.y = 1300;
			pos.x = Random::Rand(-800.0f,-400.0f);
			pos.z = Random::Rand(500.0f,700.0f);

		//	m_effectManager->AddEffectFromDataBase( 0,pos );

			pos.x = Random::Rand(-400.0f,400.0f);
			pos.z = Random::Rand( 500.0f,700.0f);

		//	m_effectManager->AddEffectFromDataBase( 1,pos);

			pos.x = Random::Rand(400.0f,800.0f);
			pos.z = Random::Rand(500.0f,700.0f);

		//	m_effectManager->AddEffectFromDataBase( 2,pos );

			m_fireTime = 0;
		}

		m_fireTime++;

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
// 描画
//=============================================================================
void ResultMaster::Draw(void)
{
}
