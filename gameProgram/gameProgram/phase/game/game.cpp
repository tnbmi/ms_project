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

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\game\gameImport.h"
#include "..\..\view\camera\camera.h"

#include "..\..\commandmanager\commandmanager.h"

#include "..\..\manager\effectManager\effectManager.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"
#include "..\..\object\player\player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\input\keyboard\keyboard.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 0.0f, -550.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Game::Game(LPDIRECT3DDEVICE9 device) : Phase(device)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_import = nullptr;
	m_camera = nullptr;

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
	// ビュー
	//----------------------------
	// カメラ
	if(!Camera::Create(&m_camera, m_device, _at, _eye))
		return false;

	// ライト

	//----------------------------
	// インポート
	//----------------------------
	if(!GameImport::Create(&m_import, m_device))
		return false;

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
	if(!DrawListManager::Create(&m_drawListManager, m_device))
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
	//m_fade = CFade::Create(device);
	//m_fade->Start(CFade::FADESTATE_IN, 1, 1.0f, 1.0f, 1.0f, 1.0f);

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
	// インポート
	//----------------------------
	SafeFinalizeDelete(m_import);

	//----------------------------
	// ビュー
	//----------------------------
	// カメラ
	SafeFinalizeDelete(m_camera);

	// ライト

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
		m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(0,0,0) );
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

	//----------------------------
	// 画面遷移
	//----------------------------
	if(pad->buttonTrigger(XINPUT_GAMEPAD_A))
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
	// カメラセット
	//----------------------------
	m_camera->SetCamera();

	//----------------------------
	// 2D描画
	//----------------------------
	m_drawListManager->AllDraw(m_camera->viewProjection());

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
	// コマンドマネージャ生成
	Commandmanager::Create(&m_command_manager, m_padXManager, m_debugproc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);

	//エフェクトマネージャ生成
	InstancingBillboard *bill;
	InstancingBillboard::Create(&bill,m_device,m_objectList,1,ObjectBase::TYPE_3D,10000,"../resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1));
	m_drawListManager->Link( bill,0,Shader::PAT_INS );
	m_updateList->Link( bill );
	EffectManager::Create( &m_effectManager,bill );
	m_effectManager->SetOption( InstancingBillboard::OPTION(true,false,false ) );
	//エフェクトロードするんごおおおおおおおおおおおお
	m_effectManager->LoadEffectData("../resources/effect/Chino.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Cocoa.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Rize.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Syaro.OEF" );

	//プレイヤー生成
	FbxModel *part1,*part2;

	FbxModel::Create( &part1,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
	FbxModel::Create( &part2,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
	//list
	m_drawListManager->Link( part1,0,Shader::PAT_FBX );
	m_updateList->Link(part1);
	m_drawListManager->Link( part2,0,Shader::PAT_FBX );
	m_updateList->Link(part2);

	part1->StartAnimation( 1,60,true );
	part2->StartAnimation( 61,91,true );

	Player::Create( &m_redTeam,part1,part2 );

	FbxModel::Create( &part1,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
	FbxModel::Create( &part2,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );

	//list
	m_drawListManager->Link( part1,0,Shader::PAT_FBX );
	m_updateList->Link(part1);
	m_drawListManager->Link( part2,0,Shader::PAT_FBX );
	m_updateList->Link(part2);

	part1->StartAnimation( 1,60,true );
	part2->StartAnimation( 61,91,true );

	Player::Create( &m_blueTeam,part1,part2 );

	m_redTeam->pos( D3DXVECTOR3( -100,0,0 ));
	m_redTeam->rot( D3DXVECTOR3(0,D3DX_PI/2,0 ) );
	m_blueTeam->pos( D3DXVECTOR3(100,0,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,150,0 ) );
	m_blueTeam->offsetPos( D3DXVECTOR3(0,150,0 ) );


	return true;
}

// EOF
