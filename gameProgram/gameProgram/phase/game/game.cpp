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
#include "..\..\sound\sound.h"
#include "..\result\result.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\game\gameImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon3D\polygon3D.h"
#include "..\..\objectBase\meshDome\meshDome.h"

#include "..\..\commandmanager\commandmanager.h"
#include "..\..\timemanager\timeManager.h"

#include "..\..\phase\game\gameMaster\gameMaster.h"
#include "..\..\object\player\player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 500.0f, 10000.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 300.0f, -2000.0f);
const int _time_max		= 3000;

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
	m_time_manager		= nullptr;
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
	Sound::Play(Sound::BGM_A);

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

	// タイム
	SafeFinalizeDelete(m_time_manager);

	//ゲームマスター
	SafeFinalizeDelete( m_gameMaster );

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
	Sound::StopAll();
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
	// タイムマネージャ
	//----------------------------
	bool transition = m_time_manager->Update();

	//transition = 1;

	//----------------------------
	//GameMaster更新
	//----------------------------
	m_gameMaster->Update();

	//----------------------------
	// 画面遷移
	//----------------------------							//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	if(transition || m_keyboard->trigger(DIK_RETURN))		// 現状タイムのみ、チームでの勝敗も(一応)追加予定
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
					D3DCOLOR_RGBA(16, 16, 16, 256), 1.0f, 0);

	//----------------------------
	// オブジェクト描画
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);

	//----------------------------
	// コマンドマネージャ描画
	//----------------------------
	m_command_manager->Draw();

	//----------------------------
	// タイムマネージャ描画
	//----------------------------
	m_time_manager->Draw();
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool Game::InitObject(void)
{
	//----------------------------
	// 地面3Dポリゴン
	//----------------------------
	Polygon3D* poly3d;
	if(!Polygon3D::Create(&poly3d, m_device, m_objectList, m_import->texture(GameImport::STONES)))
		return false;
	m_updateList->Link(poly3d);
	m_drawListManager->Link(poly3d, 4, Shader::PAT_NOR_DIR);
	poly3d->norTexture(m_import->texture(GameImport::STONES_NOR));
	poly3d->scl(512.0f*5, 512.0f*5, 0.0f);
	poly3d->rot_x(PAI * 0.5f);
	poly3d->texcoord(1, 5.0f,  0.0f);
	poly3d->texcoord(2,  0.0f, 5.0f);
	poly3d->texcoord(3, 5.0f, 5.0f);

	//----------------------------
	// 空メッシュドーム
	//----------------------------
	MeshDome* dome;
	if(!MeshDome::Create(&dome, m_device, m_objectList,
		D3DXVECTOR2(8, 3), 2500.0f, 2000.0f, m_import->texture(GameImport::SKY)))
		return false;
	m_updateList->Link(dome);
	m_drawListManager->Link(dome, 2, Shader::PAT_NONE_LIGHT);
	dome->pos_y(-150.0f);
	dome->rot_y(PAI * 0.5f);

	//----------------------------
	// コマンドマネージャ生成
	//----------------------------
	CommandManager::Create(&m_command_manager, m_padXManager, m_debugproc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);

	//----------------------------
	// タイムマネージャ生成
	//----------------------------
	TimeManager::Create(&m_time_manager, m_objectList, m_updateList, m_drawListManager, m_device, m_import, _time_max);

	//----------------------------
	//ゲームマスター生成
	//----------------------------
	GameMaster::Create( &m_gameMaster,m_device,m_objectList,m_updateList,m_drawListManager,m_import,m_debugproc,m_padXManager);

	/*
	for( int i = 0 ; i < 5 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( 600,0,100+100 * i ) );
	}

	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,500 ) );
	}
	
	for( int i = 0 ; i < 7 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,400 ) );
	}
	
	/*
	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,600 ) );
	}
	
	
	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,700 ) );
	}
	*/
	return true;
}

// EOF
