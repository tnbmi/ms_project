//*****************************************************************************
//
// ゲームフェーズ [game.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "game.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\result\result.h"

#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\game\gameImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\objectBase\polygon3D\polygon3D.h"

#include "..\..\commandmanager\commandmanager.h"

#include "..\..\manager\effectManager\effectManager.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"
#include "..\..\object\player\player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\input\keyboard\keyboard.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 150.0f, -3000.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Game::Game(LPDIRECT3DDEVICE9 device) : Phase(device)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_import = nullptr;

	m_shader = nullptr;
	m_camera = nullptr;
	m_light	 = nullptr;

	m_objectList		= nullptr;
	m_updateList		= nullptr;
	m_drawListManager	= nullptr;

	m_command_manager	= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Game::~Game(void)
{
}

//=============================================================================
// 初期化
//=============================================================================
bool Game::Initialize(void)
{
	//----------------------------
	// インポート
	//----------------------------
	if(!GameImport::Create(&m_import, m_device))
		return false;

	//----------------------------
	// シェーダー
	//----------------------------
	if(!Shader::Create(&m_shader, m_device))
		return false;

	//----------------------------
	// ビュー
	//----------------------------
	// カメラ
	if(!Camera::Create(&m_camera, m_device, _at, _eye))
		return false;

	// ライト
	if(!Light::Create(&m_light, m_device))
		return false;
	m_light->dirLightVector(-1.0f, -3.0f, 2.0f);

	//----------------------------
	// 管理リスト
	//----------------------------
	// オブジェクトリスト
	if(!ObjectList::Create(&m_objectList))
		return false;

	// 更新リスト
	if(!UpdateList::Create(&m_updateList))
		return false;

	// 描画リスト
	if(!DrawListManager::Create(&m_drawListManager, m_device, m_shader))
		return false;

	//----------------------------
	// オブジェクト
	//----------------------------
	if(!InitObject())
		return false;

	//----------------------------
	// サウンドの再生
	//----------------------------

	//----------------------------
	// フェード
	//----------------------------
	//Fade::Create( &m_fade , m_updateList , m_drawListManager , m_device , m_objectList , m_import );
	//m_fade->Start(Fade::FADESTATE_IN, 10, 1.0f, 1.0f, 1.0f, 0.0f);

	//----------------------------
	// ステータス初期化
	//----------------------------

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Game::Finalize(void)
{
	//----------------------------
	// オブジェクト
	//----------------------------
	// 存在するオブジェクト全て削除
	m_objectList->AllDarelete();

	// 描画リストマネージャー
	SafeFinalizeDelete(m_drawListManager);

	// 更新リスト
	SafeDelete(m_updateList);

	// オブジェクトリスト
	SafeDelete(m_objectList);

	// コマンド
	SafeFinalizeDelete(m_command_manager);

	//エフェクト
	m_effectManager->Finalize();
	SafeFinalizeDelete( m_effectManager );

	//プレイヤー削除
	m_redTeam->Finalize();
	m_blueTeam->Finalize();
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );

	//----------------------------
	// ビュー
	//----------------------------
	// カメラ
	SafeFinalizeDelete(m_camera);

	// ライト
	SafeFinalizeDelete(m_light);

	//----------------------------
	// シェーダー
	//----------------------------
	SafeFinalizeDelete(m_shader);

	//----------------------------
	// インポート
	//----------------------------
	SafeFinalizeDelete(m_import);

	//----------------------------
	// サウンドの停止
	//----------------------------

}

//=============================================================================
// 更新
//=============================================================================
void Game::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===ゲームフェーズ===\n");
#endif

	m_camera->SetCamera();
	//----------------------------
	// 更新内容
	//----------------------------
	PadX* pad = m_padXManager->pad(0);

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	//----------------------------
	// コマンドマネージャ
	//----------------------------
	m_command_manager->Update();

	//----------------------------
	//Player更新
	//----------------------------
	m_redTeam->Update();
	m_blueTeam->Update();

	//----------------------------
	//EffectManager更新
	//----------------------------
	m_effectManager->Update();

	if( m_keyboard->trigger(DIK_1 ) )
	{
		m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(0,800,100) );
		m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(-800,1000,100) );
		m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(800,1000,100) );
	}

	if( m_keyboard->trigger(DIK_2 ) )
	{
		m_effectManager->AddEffectFromDataBase(1,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_3 ) )
	{
		m_effectManager->AddEffectFromDataBase(2,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_4 ) )
	{
		m_effectManager->AddEffectFromDataBase(3,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_5 ) )
	{
		m_effectManager->AddEffectFromDataBase(4,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_6 ) )
	{
		m_effectManager->AddEffectFromDataBase(5,D3DXVECTOR3(0,60,0) );
	}

	if( m_keyboard->trigger(DIK_7 ) )
	{
		m_effectManager->AddEffectFromDataBase(6,D3DXVECTOR3(0,60,0) );
	}

	if( m_keyboard->trigger( DIK_0 ) )
	{
		m_redTeam->Move( D3DXVECTOR3( -500,0,0 ),D3DXVECTOR3(-30,0,0),500 );
		m_blueTeam->Move( D3DXVECTOR3( 500,0,0 ),D3DXVECTOR3(30,0,0),500 );
	}

	//----------------------------
	// 画面遷移
	//----------------------------
	
	if(m_command_manager->GetState() == Commandmanager::TEAM0_WIN || 
	   m_command_manager->GetState() == Commandmanager::TEAM1_WIN )
	{
		Manager::nextPhase((Phase*)new Result(m_device));
	}
}

//=============================================================================
// 描画
//=============================================================================
void Game::Draw(void)
{
	//----------------------------
	// バックバッファ＆Ｚバッファのクリア
	//----------------------------
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(64, 64, 128, 256), 1.0f, 0);

	//----------------------------
	// オブジェクト描画
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);

	//----------------------------
	// コマンドマネージャ描画
	//----------------------------
	m_command_manager->Draw();
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool Game::InitObject(void)
{
	//----------------------------
	// コマンドマネージャ生成
	//----------------------------
	Commandmanager::Create(&m_command_manager, m_padXManager, m_debugproc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);

	//----------------------------
	// 3Dポリゴンテスト
	//----------------------------
	
	Polygon3D* poly3d;
	if(!Polygon3D::Create(&poly3d, m_device, m_objectList, m_import->texture(GameImport::TEST_0)))
		return false;
	m_updateList->Link(poly3d);
	m_drawListManager->Link(poly3d, 4, Shader::PAT_LIGHT);
	poly3d->scl(512.0f, 512.0f, 0.0f);
	poly3d->rot_x(PAI * 0.5f);
	
	//----------------------------
	//エフェクトマネージャ生成
	//----------------------------
	
	InstancingBillboard *bill;
	InstancingBillboard::Create(&bill,m_device,m_objectList,1,ObjectBase::TYPE_3D,10000,"../resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1));
	m_drawListManager->Link( bill,0,Shader::PAT_INS );
	m_updateList->Link( bill );
	EffectManager::Create( &m_effectManager,bill );
	m_effectManager->SetOption( InstancingBillboard::OPTION(true,false,false ) );
	//エフェクトロードするんごおおおおおおおおおおおお
	m_effectManager->LoadEffectData("../resources/effect/FireWorks.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Cocoa.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Rize.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Syaro.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya2.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya3.OEF" );
	
	
	//----------------------------
	//プレイヤー生成
	//----------------------------
	Player::Create( &m_blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"../resources/fbxModel/daisya.bin","../resources/fbxModel/jiorama.bin","../resources/fbxModel/robo2.bin");
	Player::Create( &m_redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"../resources/fbxModel/daisya.bin","../resources/fbxModel/jiorama.bin","../resources/fbxModel/robo2.bin");
	m_redTeam->rot( D3DXVECTOR3(0,0,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,-D3DX_PI/2,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,232.059,0 ) );
	m_blueTeam->offsetPos( D3DXVECTOR3(0,232.059,0 ) );

	m_redTeam->Move( D3DXVECTOR3(-1000,0,0),D3DXVECTOR3(-500,0,0),300 );
	m_blueTeam->Move( D3DXVECTOR3(1000,0,0),D3DXVECTOR3(500,0,0),300 );

	//---------------------------
	//
	//---------------------------

	for( int i = 0 ; i < 5 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -600,0,100+100 * i ) );
	}
	/*
	for( int i = 0 ; i < 5 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( 600,0,100+100 * i ) );
	}

	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,500 ) );
	}
	
	for( int i = 0 ; i < 7 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,400 ) );
	}
	
	/*
	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,600 ) );
	}
	
	
	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,700 ) );
	}
	*/
	return true;
}

// EOF
