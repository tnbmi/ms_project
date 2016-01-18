//*****************************************************************************
//
// デモフェーズ [demo.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "demo.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\..\sound\sound.h"
#include "..\title\title.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\demo\demoImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\commandmanager\commandmanager.h"
#include "..\..\timemanager\timeManager.h"

#include "..\..\objectBase\polygon3D\polygon3D.h"
#include "..\..\objectBase\meshDome\meshDome.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"

#include "..\..\object\player\player.h"

#include "..\..\phase\demo\demoMaster\demoMaster.h"
#include "..\..\import\fbx\fbxTexImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 500.0f, -200.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 300.0f, -700.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Demo::Demo(LPDIRECT3DDEVICE9 device) : Phase(device)
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
	m_fbxTexImport = nullptr;

	m_demoMaster = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Demo::~Demo(void)
{
}

//=============================================================================
// 初期化
//=============================================================================
bool Demo::Initialize(void)
{
	//----------------------------
	// インポート
	//----------------------------
	if(!DemoImport::Create(&m_import, m_device))
		return false;
	m_fbxTexImport = Manager::fbxTexImport();

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
	// サウンドの停止
	//----------------------------
	Sound::StopAll();

	//----------------------------
	// サウンドの再生
	//----------------------------
	Sound::Play(Sound::BGM_GAME01);

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Demo::Finalize(void)
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

	//デモマスター
	SafeFinalizeDelete( m_demoMaster );

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
void Demo::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===デモフェーズ===\n");
#endif

	//----------------------------
	// カメラ設定
	//----------------------------
	m_camera->SetCamera();

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	//----------------------------
	//DemoMaster更新
	//----------------------------
	if(m_demoMaster->Update() || m_padXManager->InputChkTrigger() || m_keyboard->trigger(DIK_RETURN))
		Manager::nextPhase((Phase*)new Title(m_device));
}

//=============================================================================
// 描画
//=============================================================================
void Demo::Draw(void)
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
bool Demo::InitObject(void)
{
	//----------------------------
	// スコア初期化
	//----------------------------
	Manager::score(0, 0);

	//----------------------------
	// 空メッシュドーム
	//----------------------------
	MeshDome* dome;
	if(!MeshDome::Create(&dome, m_device, m_objectList,
		D3DXVECTOR2(8, 3), 2500.0f, 2000.0f, m_import->texture(DemoImport::SKY)))
		return false;
	m_updateList->Link(dome);
	m_drawListManager->Link(dome, 0, Shader::PAT_NONE_LIGHT);
	dome->pos_y(-150.0f);
	dome->rot_y(PAI * 0.5f);

	//----------------------------
	// 背景3Dポリゴン
	//----------------------------
	Polygon3D* bg3D;
	if(!Polygon3D::Create(&bg3D, m_device, m_objectList, m_import->texture(DemoImport::BG)))
		return false;
	m_updateList->Link(bg3D);
	m_drawListManager->Link(bg3D, 1, Shader::PAT_NONE_LIGHT);
	D3DXVECTOR3 size = D3DXVECTOR3(1800.0f*0.6f, 720.0f*0.6f, 0.0f);
	bg3D->scl(size);
	bg3D->pos(0.0f, size.y*0.5f, 512.0f*2.5f);
	bg3D->texcoord(0, 0.0f, 0.01f);
	bg3D->texcoord(1, 1.0f, 0.01f);

	//----------------------------
	// 地面3Dポリゴン
	//----------------------------
	Polygon3D* ground;
	if(!Polygon3D::Create(&ground, m_device, m_objectList, m_import->texture(DemoImport::GROUND)))
		return false;
	m_updateList->Link(ground);
	m_drawListManager->Link(ground, 2, Shader::PAT_NONE_LIGHT);
	ground->scl(512.0f*5, 512.0f*5, 0.0f);
	ground->rot_x(PAI * 0.5f);

	//----------------------------
	//デモマスター生成
	//----------------------------
	DemoMaster::Create( &m_demoMaster,m_device,m_objectList,m_updateList,m_drawListManager,
						m_import,m_fbxTexImport,m_debugproc,m_padXManager,m_camera );

	return true;
}

// EOF
