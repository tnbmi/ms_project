//*****************************************************************************
//
// タイトルフェーズ [title.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "title.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\demo\demo.h"

#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\title\titleImport.h"
#include "..\..\view\camera\camera.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\commandmanager\commandmanager.h"
#include "..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\objectBase\polygon3D\polygon3D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 0.0f, -50.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Title::Title(LPDIRECT3DDEVICE9 device) : Phase(device)
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
Title::~Title(void)
{
}

//=============================================================================
// 初期化
//=============================================================================
bool Title::Initialize(void)
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
	if(!TitleImport::Create(&m_import, m_device))
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
	//m_fade = CFade::Create(m_device);
	//m_fade->Start(CFade::FADESTATE_IN, 1, 1.0f, 1.0f, 1.0f, 1.0f);

	//----------------------------
	// ステータス初期化
	//----------------------------

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Title::Finalize(void)
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
void Title::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===タイトルフェーズ===\n");
#endif

	//----------------------------
	// 更新内容
	//----------------------------
	PadX* pad = m_padXManager->pad(0);

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	m_command_manager->Update();

	//----------------------------
	// 画面遷移
	//----------------------------
	if(pad->buttonTrigger(XINPUT_GAMEPAD_A))
	{
	//	Manager::nextPhase((Phase*)new Demo(m_device));
	}
}

//=============================================================================
// 描画
//=============================================================================
void Title::Draw(void)
{
	//----------------------------
	// バックバッファ＆Ｚバッファのクリア
	//----------------------------
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(32, 0, 0, 255), 1.0f, 0);

	//----------------------------
	// 2D描画
	//----------------------------
	m_drawListManager->AllDraw(m_camera->viewProjection());

	m_command_manager->Draw();
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool Title::InitObject(void)
{
	Commandmanager::Create(&m_command_manager, m_padXManager, m_debugproc);

	//----------------------------
	// 2Dポリゴンテスト
	//----------------------------
	Polygon2D* poly2d;
	if(!Polygon2D::Create(&poly2d, m_device, m_objectList, m_import->texture(TitleImport::TEST_0)))
		return false;
	m_updateList->Link(poly2d);
	m_drawListManager->Link(poly2d, 4, Shader::PAT_2D);
	poly2d->pos(200.0f, 200.0f, 0.0f);
	poly2d->scl(128.0f, 128.0f, 0.0f);

	if(!Polygon2D::Create(&poly2d, m_device, m_objectList, m_import->texture(TitleImport::TEST_0)))
		return false;
	m_updateList->Link(poly2d);
	m_drawListManager->Link(poly2d, 4, Shader::PAT_2D);
	poly2d->pos(250.0f, 250.0f, 0.0f);

	//----------------------------
	// 3Dポリゴンテスト
	//----------------------------
	Polygon3D* poly3d;
	if(!Polygon3D::Create(&poly3d, m_device, m_objectList, m_import->texture(TitleImport::TEST_0)))
		return false;
	m_updateList->Link(poly3d);
	m_drawListManager->Link(poly3d, 4, Shader::PAT_LIGHT);
	//poly2d->pos(200.0f, 200.0f, 0.0f);
	poly3d->scl(128.0f, 128.0f, 0.0f);

	return true;
}

// EOF
