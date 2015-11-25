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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 50.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 100.0f, -250.0f);

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
	if(!Light::Create(&m_light, m_device, m_shader->vsc(1)))
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
	// ビューセット
	//----------------------------
	// カメラ
	m_camera->SetCamera();

	// ライト
	m_light->SetLight();

	//----------------------------
	// オブジェクト描画
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
	poly3d->pos(30.0f, 50.0f, -10.0f);
	poly3d->scl(32.0f, 32.0f, 0.0f);

	return true;
}

// EOF
