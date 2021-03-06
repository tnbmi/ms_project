//*****************************************************************************
//
// 準備フェーズ [standby.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "standby.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\..\sound\sound.h"
#include "..\game\game.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\standby\standbyImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"
#include "..\..\phase\standby\standbyMaster\standbyMaster.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 100.0f, 800.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 150.0f, -700.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Standby::Standby(LPDIRECT3DDEVICE9 device) : Phase(device)
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

	m_standbyMaster = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Standby::~Standby(void)
{
}

//=============================================================================
// 初期化
//=============================================================================
bool Standby::Initialize(void)
{
	//----------------------------
	// インポート
	//----------------------------
	if(!StandbyImport::Create(&m_import, m_device))
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
	// サウンドの停止
	//----------------------------
	Sound::StopAll();

	//----------------------------
	// サウンドの再生
	//----------------------------
	Sound::Play(Sound::BGM_STANDBY02);

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Standby::Finalize(void)
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
	SafeFinalizeDelete( m_standbyMaster );

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
}

//=============================================================================
// 更新
//=============================================================================
void Standby::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===準備フェーズ===\n");
#endif

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	bool isStandby = m_standbyMaster->Update();

	//----------------------------
	// 画面遷移
	//----------------------------
	if(isStandby)
		Manager::nextPhase((Phase*)new Game(m_device));
}

//=============================================================================
// 描画
//=============================================================================
void Standby::Draw(void)
{
	//----------------------------
	// バックバッファ＆Ｚバッファのクリア
	//----------------------------
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//----------------------------
	// オブジェクト描画
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool Standby::InitObject(void)
{
	// ダミーフラグを初期化
	Manager::dummyFlg(0, false);
	Manager::dummyFlg(1, false);
	Manager::dummyFlg(2, false);
	Manager::dummyFlg(3, false);

	// 準備マスター
	StandbyMaster::Create( &m_standbyMaster,m_device,m_objectList,m_updateList,m_drawListManager,m_import,m_debugproc,m_padXManager,m_light );
	m_standbyMaster->keyboard(m_keyboard);

	return true;
}

// EOF
