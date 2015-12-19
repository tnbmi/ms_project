//*****************************************************************************
//
// 準備画面管理 [standbyMaster.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "standbyMaster.h"
#include "..\..\..\common\safe.h"

#include "..\..\..\manager\manager.h"
#include "..\..\..\import\main\mainImport.h"

#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"
#include "..\..\..\score\score.h"
#include "..\..\..\effectManager\effectManager.h"
#include "..\..\..\object\player\player.h"
#include "..\..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\..\common\complement\complement.h"

#include "..\..\..\view\light\light.h"
#include "..\..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\..\import\standby\StandbyImport.h"

#include "..\..\..\common\complement\complement.h"
#include "..\..\..\input\padX\padXManager.h"
#include "..\..\..\input\padX\padX.h"
#include "..\..\..\ggy2DAnimationManager\ggy2DAnimationManager.h"
#include "..\..\..\sound\sound.h"


//=============================================================================
// コンストラクタ
//=============================================================================
StandbyMaster::StandbyMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,StandbyImport *import,Debugproc *proc,PadXManager* padXMaster,Light *light )
{
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	m_light = light;
}

//=============================================================================
// デストラクタ
//=============================================================================
StandbyMaster::~StandbyMaster(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool StandbyMaster::Create(StandbyMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						StandbyImport* import,Debugproc* debugproc,PadXManager* padXManager,Light *light)
{

	StandbyMaster* pointer = new StandbyMaster( device,objectList,updateList,drawList,import,debugproc,padXManager,light );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool StandbyMaster::Initialize(void)
{
	Polygon2D::Create( &m_back,m_device,m_objectList,m_import->texture( StandbyImport::BACK ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_mid,m_device,m_objectList,m_import->texture( StandbyImport::MID ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_front,m_device,m_objectList,m_import->texture( StandbyImport::FRONT ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueTeamStandby[0].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_BLUE ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueTeamStandby[1].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_BLUE ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_redTeamStandby[0].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_RED ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_redTeamStandby[1].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_RED ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_battle,m_device,m_objectList,m_import->texture( StandbyImport::BATTLE ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_redLogo.pol,m_device,m_objectList,m_import->texture( StandbyImport::SELECT_RED ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueLogo.pol,m_device,m_objectList,m_import->texture( StandbyImport::SELECT_BLUE ),ObjectBase::TYPE_2D );



	MainImport* mainImport = Manager::mainImport();
	Ggy2DAnimationManager::Create( &m_ggyBlueAnimManager,m_device,m_objectList,m_updateList,m_drawListManager );

	m_ggyBlueAnimManager->SetTexture( 0,mainImport->texture(MainImport::GGYBLUE_WAIT) );
	m_ggyBlueAnimManager->SetTexture( 8,mainImport->texture(MainImport::GGYBLUE_POSE) );
	m_ggyBlueAnimManager->pos(D3DXVECTOR3( 330,420,0 ));
	m_ggyBlueAnimManager->scl(D3DXVECTOR3( 320,256,0 ));

	m_ggyBlueAnimManager->StartAnimation(0,true);

	Ggy2DAnimationManager::Create( &m_ggyRedAnimManager,m_device,m_objectList,m_updateList,m_drawListManager );

	m_ggyRedAnimManager->SetTexture( 0,mainImport->texture(MainImport::GGYRED_WAIT) );
	m_ggyRedAnimManager->SetTexture( 8,mainImport->texture(MainImport::GGYRED_POSE) );
	m_ggyRedAnimManager->pos(D3DXVECTOR3( 950,420,0 ));
	m_ggyRedAnimManager->scl(D3DXVECTOR3( 320,256,0 ));

	m_ggyRedAnimManager->StartAnimation(0,true);



	//背景リンク
	m_updateList->Link( m_back );
	m_drawListManager->Link( m_back,0,Shader::PAT_2D );

	m_updateList->Link( m_mid );
	m_drawListManager->Link( m_mid,1,Shader::PAT_2D );

	m_updateList->Link( m_front );
	m_drawListManager->Link( m_front,2,Shader::PAT_2D );

	//スタンバイシートリンク
	m_updateList->Link( m_blueTeamStandby[0].pol );
	m_drawListManager->Link( m_blueTeamStandby[0].pol,2,Shader::PAT_2D );
	m_updateList->Link( m_blueTeamStandby[1].pol );
	m_drawListManager->Link( m_blueTeamStandby[1].pol,2,Shader::PAT_2D );

	m_updateList->Link( m_redTeamStandby[0].pol );
	m_drawListManager->Link( m_redTeamStandby[0].pol,2,Shader::PAT_2D );
	m_updateList->Link( m_redTeamStandby[1].pol );
	m_drawListManager->Link( m_redTeamStandby[1].pol,2,Shader::PAT_2D );

	//勝負ロゴリンク
	m_updateList->Link( m_battle );
	m_drawListManager->Link( m_battle,2,Shader::PAT_2D );

	//ロゴ
	m_updateList->Link( m_blueLogo.pol );
	m_drawListManager->Link( m_blueLogo.pol,2,Shader::PAT_2D );
	m_updateList->Link( m_redLogo.pol );
	m_drawListManager->Link( m_redLogo.pol,2,Shader::PAT_2D );

	//スケーリング指定(固定)
	m_back->scl( SCREEN_WIDTH,SCREEN_HEIGHT,1 );
	m_mid->scl( SCREEN_WIDTH,SCREEN_HEIGHT,1 );
	m_front->scl( SCREEN_WIDTH,SCREEN_HEIGHT,1 );

	//位置指定(固定)
	m_back->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0 );
	m_mid->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0 );
	m_front->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0 );

	//スタンバイ
	m_blueTeamStandby[0].pol->scl( 0.0f,0.0f,1.0f );
	m_blueTeamStandby[1].pol->scl( 0.0f,0.0f,1.0f );

	m_redTeamStandby[0].pol->scl( 0.0f,0.0f,1.0f );
	m_redTeamStandby[1].pol->scl( 0.0f,0.0f,1.0f );

	//235  593
	//452  593
	//832  593
	//1059 593
	m_blueTeamStandby[0].pol->pos( 235.0f,593.0f,0.0f );
	m_blueTeamStandby[1].pol->pos( 452.0f,593.0f,0.0f );
	m_redTeamStandby[0].pol->pos( 832.0f,593.0f,0.0f );
	m_redTeamStandby[1].pol->pos( 1059.0f,593.0f,0.0f );

	//勝負ロゴ
	m_battle->scl(0,0,1 );
	m_battle->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0 );

	//ロゴ
	m_blueLogo.pol->pos( 340.0f,689.0f,0.0f);
	m_blueLogo.pol->scl(417.0f,59.0f,1.0f );
	m_blueLogo.pol->texcoord(0,0.0f,0.0f);
	m_blueLogo.pol->texcoord(1,1.0f,0.111f);
	m_blueLogo.pol->texcoord(2,0.0f,0.0f);
	m_blueLogo.pol->texcoord(3,1.0f,0.111f);
	m_blueLogo.uv[0] = D3DXVECTOR2(0.0f,0.0f);
	m_blueLogo.uv[1] = D3DXVECTOR2(1.0f,0.0f);
	m_blueLogo.uv[2] = D3DXVECTOR2(0.0f,0.111f);
	m_blueLogo.uv[3] = D3DXVECTOR2(1.0f,0.111f);
	m_blueLogo.offsetUv = D3DXVECTOR2(0.0f,0.111f);
	m_blueLogo.animSum = 9;
	m_blueLogo.time = 0;

	m_redLogo.pol->pos(944.0f,689.0f,0.0f);
	m_redLogo.pol->scl(417.0f,59.0f,1.0f);
	m_redLogo.pol->texcoord(0,0.0f,0.0f);
	m_redLogo.pol->texcoord(1,1.0f,0.111f);
	m_redLogo.pol->texcoord(2,0.0f,0.0f);
	m_redLogo.pol->texcoord(3,1.0f,0.111f);
	m_redLogo.uv[0] = D3DXVECTOR2(0.0f,0.0f);
	m_redLogo.uv[1] = D3DXVECTOR2(1.0f,0.111f);
	m_redLogo.uv[2] = D3DXVECTOR2(0.0f,0.0f);
	m_redLogo.uv[3] = D3DXVECTOR2(1.0f,0.111f);
	m_redLogo.offsetUv = D3DXVECTOR2(0.0f,0.111f);

	//初期化
	m_blueTeamStandby[0].isStandby = false;
	m_blueTeamStandby[0].time = _compFrame;
	m_blueTeamStandby[1].isStandby = false;
	m_blueTeamStandby[1].time = _compFrame;

	m_redTeamStandby[0].isStandby = false;
	m_redTeamStandby[0].time = _compFrame;
	m_redTeamStandby[1].isStandby = false;
	m_redTeamStandby[1].time = _compFrame;
	
	m_compTime = 0;

	m_phase = PHASE_WAIT;
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void StandbyMaster::Finalize(void)
{
	m_blueTeamStandby[0].isStandby = false;
	m_blueTeamStandby[0].time = 0;
	m_blueTeamStandby[1].isStandby = false;
	m_blueTeamStandby[1].time = 0;

	m_redTeamStandby[0].isStandby = false;
	m_redTeamStandby[0].time = 0;
	m_redTeamStandby[1].isStandby = false;
	m_redTeamStandby[1].time = 0;

	SafeFinalizeDelete( m_ggyRedAnimManager );
	SafeFinalizeDelete( m_ggyBlueAnimManager );
}

//=============================================================================
// 更新
//=============================================================================
bool StandbyMaster::Update(void)
{

	D3DXVECTOR3 battleScl;

	int animNum = (int)( m_blueLogo.time / 2 ) % m_blueLogo.animSum;

	D3DXVECTOR2 offset = m_blueLogo.offsetUv * (float)animNum;

	m_blueLogo.time++;

	if( animNum >= m_blueLogo.animSum-1 )
	{
		m_blueLogo.time--;
	}

	m_blueLogo.pol->texcoord(0,m_blueLogo.uv[0].x + offset.x,m_blueLogo.uv[0].y + offset.y );
	m_blueLogo.pol->texcoord(1,m_blueLogo.uv[1].x + offset.x,m_blueLogo.uv[1].y + offset.y );
	m_blueLogo.pol->texcoord(2,m_blueLogo.uv[2].x + offset.x,m_blueLogo.uv[2].y + offset.y );
	m_blueLogo.pol->texcoord(3,m_blueLogo.uv[3].x + offset.x,m_blueLogo.uv[3].y + offset.y );

	m_redLogo.pol->texcoord(0,m_blueLogo.uv[0].x + offset.x,m_blueLogo.uv[0].y + offset.y );
	m_redLogo.pol->texcoord(1,m_blueLogo.uv[1].x + offset.x,m_blueLogo.uv[1].y + offset.y );
	m_redLogo.pol->texcoord(2,m_blueLogo.uv[2].x + offset.x,m_blueLogo.uv[2].y + offset.y );
	m_redLogo.pol->texcoord(3,m_blueLogo.uv[3].x + offset.x,m_blueLogo.uv[3].y + offset.y );

	switch( m_phase )
	{
	case PHASE_WAIT:
		//１フレ前のスタンバイ情報保持
		m_blueTeamStandby[0].isPrevStandby = m_blueTeamStandby[0].isStandby;
		m_blueTeamStandby[1].isPrevStandby = m_blueTeamStandby[1].isStandby;
		m_redTeamStandby[0].isPrevStandby = m_redTeamStandby[0].isStandby;
		m_redTeamStandby[1].isPrevStandby = m_redTeamStandby[1].isStandby;

		//キー判定　青
		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_padXManager->pad(i)->buttonTrigger( XINPUT_GAMEPAD_START ) )
			{
				m_blueTeamStandby[i].isStandby = true;
			}
			else if( m_padXManager->pad(i)->buttonTrigger( XINPUT_GAMEPAD_BACK ) )
			{
				m_blueTeamStandby[i].isStandby = false;
			}
		}


		//キー判定　赤
		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_padXManager->pad(i+2)->buttonTrigger( XINPUT_GAMEPAD_START ) )
			{
				m_redTeamStandby[i].isStandby = true;
			}
			else if( m_padXManager->pad(i+2)->buttonTrigger( XINPUT_GAMEPAD_BACK ) )
			{
				m_redTeamStandby[i].isStandby = false;
			}
		}

		bool isDiffStandby[4];
		isDiffStandby[0] = false;
		isDiffStandby[1] = false;
		isDiffStandby[2] = false;
		isDiffStandby[3] = false;

		//状況が変われば再補完
		if( m_blueTeamStandby[0].isPrevStandby != m_blueTeamStandby[0].isStandby )
		{
			m_blueTeamStandby[0].time = 0;
			isDiffStandby[0] = true;
		}
			
		if( m_blueTeamStandby[1].isPrevStandby != m_blueTeamStandby[1].isStandby )
		{
			m_blueTeamStandby[1].time = 0;
			isDiffStandby[1] = true;
		}

		if( m_redTeamStandby[0].isPrevStandby != m_redTeamStandby[0].isStandby )
		{
			m_redTeamStandby[0].time = 0;
			isDiffStandby[2] = true;
		}

		if( m_redTeamStandby[1].isPrevStandby != m_redTeamStandby[1].isStandby )
		{
			m_redTeamStandby[1].time = 0;
			isDiffStandby[3] = true;
		}

		if( isDiffStandby[0] || isDiffStandby[1] )
		{
			if( m_blueTeamStandby[0].isStandby && m_blueTeamStandby[1].isStandby )
			{
				m_ggyBlueAnimManager->StartAnimation(8,false );
			}
			else
			{
				m_ggyBlueAnimManager->StartAnimation(0,true );
			}

		}

		if( isDiffStandby[2] || isDiffStandby[3] )
		{
			if( m_redTeamStandby[0].isStandby && m_redTeamStandby[1].isStandby )
			{
				m_ggyRedAnimManager->StartAnimation(8,false );
			}
			else
			{
				m_ggyRedAnimManager->StartAnimation(0,true );
			}
		}



		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_blueTeamStandby[i].isStandby )
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1.0f ),0,(float)_compFrame,m_blueTeamStandby[i].time,Cube );
				m_blueTeamStandby[i].pol->scl(scl);

				m_blueTeamStandby[i].time++;

				if( m_blueTeamStandby[i].time > _compFrame )
				{
					m_blueTeamStandby[i].time = _compFrame;
				}
			}
			else
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1.0f ),D3DXVECTOR3( 0.0f,0.0f,0.0f ),0,(float)_compFrame,m_blueTeamStandby[i].time,Cube );
				m_blueTeamStandby[i].pol->scl(scl);

				m_blueTeamStandby[i].time++;

				if( m_blueTeamStandby[i].time > _compFrame )
				{
					m_blueTeamStandby[i].time = _compFrame;
				}
			}
		}

		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_redTeamStandby[i].isStandby )
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1 ),0,(float)_compFrame,m_redTeamStandby[i].time,Cube );
				m_redTeamStandby[i].pol->scl(scl);

				m_redTeamStandby[i].time++;

				if( m_redTeamStandby[i].time > _compFrame )
				{
					m_redTeamStandby[i].time = _compFrame;
				}
			}
			else
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1 ),D3DXVECTOR3( 0.0f,0.0f,0.0f ),0,(float)_compFrame,m_redTeamStandby[i].time,Cube );
				m_redTeamStandby[i].pol->scl(scl);

				m_redTeamStandby[i].time++;

				if( m_redTeamStandby[i].time > _compFrame )
				{
					m_redTeamStandby[i].time = _compFrame;
				}
			}
		}

		//メンバー全てが準備完了なら
		if( m_blueTeamStandby[0].isStandby && m_blueTeamStandby[0].isStandby && m_redTeamStandby[0].isStandby && m_redTeamStandby[1].isStandby
			&& m_blueTeamStandby[0].time >= _compFrame && m_blueTeamStandby[1].time >= _compFrame 
			&& m_redTeamStandby[0].time >= _compFrame  && m_redTeamStandby[1].time >= _compFrame)
		{
			m_phase = PHASE_MOTION;
			m_ggyRedAnimManager->StartAnimation(8,false);
			m_ggyBlueAnimManager->StartAnimation(8,false);
		}

		break;

	case PHASE_MOTION:

		//赤も青も同じフレーム数なので赤で判定しとく
		if( m_ggyRedAnimManager->isEndAnimation() )
		{
			m_phase = PHASE_STANDBY;
		}

		
		break;

	case PHASE_STANDBY:

		battleScl = LerpVec3( D3DXVECTOR3( 781.0f*4.0f,413.0f*4.0f,1.0f ),D3DXVECTOR3( 781.0f/2.0f,413.0f/2.0f,0.0f ),0,_compFrame*2,m_compTime,Cube );

		m_battle->scl(battleScl);

		m_compTime++;

		if( m_compTime > _compFrame*2 )
		{
			m_compTime = _compFrame*2;
			m_phase = PHASE_NEXTSCENE;
		}

		break;

	case PHASE_NEXTSCENE:

		return true;

		break;

	}

	m_ggyRedAnimManager->Update();
	m_ggyBlueAnimManager->Update();

	return false;
}

//=============================================================================
// 描画
//=============================================================================
void StandbyMaster::Draw(void)
{
}
