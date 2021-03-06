//*****************************************************************************
//
// リザルトフェーズ [result.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "result.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\..\sound\sound.h"
#include "..\title\title.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\result\resultImport.h"
#include "..\..\view\camera\camera.h"

#include "..\..\shader\shader.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon3D\polygon3D.h"
#include "..\..\objectBase\meshDome\meshDome.h"

#include "resultMaster\resultMaster.h"
#include "..\..\import\fbx\fbxTexImport.h"
#include "..\..\sound\sound.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 1000.0f, 2000.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 120.0f, -600.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Result::Result(LPDIRECT3DDEVICE9 device) : Phase(device)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_import = nullptr;
	m_camera = nullptr;
	m_resultMaster = nullptr;
	m_fbxTexImport = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Result::~Result(void)
{
}

//=============================================================================
// 初期化
//=============================================================================
bool Result::Initialize(void)
{
	//----------------------------
	// インポート
	//----------------------------
	if(!ResultImport::Create(&m_import, m_device))
		return false;

	//if(!FbxTexImport::Create(&m_fbxTexImport,m_device ))
	//	return false;
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
	m_light->dirLightAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	m_light->dirLightDiffuse(0.3f, 0.3f, 0.3f, 1.0f);
	m_light->dirLightVector(0.0f, -1.0f, -2.0f);

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
	Sound::Play(Sound::BGM_RESULT01 );

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Result::Finalize(void)
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
	SafeFinalizeDelete(m_light);

	//----------------------------
	// シェーダー
	//----------------------------
	SafeFinalizeDelete(m_shader);

	//----------------------------
	//リザルトマスター
	//----------------------------
	SafeFinalizeDelete( m_resultMaster );
}

//=============================================================================
// 更新
//=============================================================================
void Result::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===リザルトフェーズ===\n");
#endif

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	//----------------------------
	//リザルトマスター更新
	//----------------------------
	m_resultMaster->Update();

	//----------------------------
	// 画面遷移
	//----------------------------
	if((m_padXManager->InputChkPress(XINPUT_GAMEPAD_LEFT_THUMB ) && m_padXManager->InputChkPress(XINPUT_GAMEPAD_START))
	|| m_keyboard->trigger(DIK_RETURN))
	{
		Manager::nextPhase((Phase*)new Title(m_device));
	}
}

//=============================================================================
// 描画
//=============================================================================
void Result::Draw(void)
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
bool Result::InitObject(void)
{
	//----------------------------
	// 空メッシュドーム
	//----------------------------
	
	MeshDome* dome;
	//if(!MeshDome::Create(&dome, m_device, m_objectList,
	//	D3DXVECTOR2(8, 7), 2000.0f, 2500.0f, m_import->texture(ResultImport::SKY)))

	if(!MeshDome::Create(&dome, m_device, m_objectList,
		D3DXVECTOR2(8, 7), 2500.0f, 2500.0f, m_import->texture(ResultImport::SKY)))
		return false;
	m_updateList->Link(dome);
	m_drawListManager->Link(dome, 0, Shader::PAT_NONE_LIGHT);
	dome->pos_y(-150.0f);
	dome->rot_y(PAI * 0.5f);
	
	//----------------------------
	// 背景3Dポリゴン
	//----------------------------
	/*Polygon3D* bg3D;
	if(!Polygon3D::Create(&bg3D, m_device, m_objectList, m_import->texture(ResultImport::BG)))
		return false;
	m_updateList->Link(bg3D);
	m_drawListManager->Link(bg3D, 1, Shader::PAT_NONE_LIGHT);
	D3DXVECTOR3 size = D3DXVECTOR3(1800.0f, 720.0f, 0.0f);
	bg3D->scl(size);
	bg3D->pos(0.0f, size.y*0.5f, 512.0f*2.5f);
	bg3D->texcoord(0, 0.0f, 0.01f);
	bg3D->texcoord(1, 1.0f, 0.01f);
	
	//----------------------------
	// 地面3Dポリゴン
	//----------------------------
	Polygon3D* ground;
	if(!Polygon3D::Create(&ground, m_device, m_objectList, m_import->texture(ResultImport::GROUND)))
		return false;
	m_updateList->Link(ground);
	m_drawListManager->Link(ground, 2, Shader::PAT_NONE_LIGHT);
	ground->scl(512.0f*5, 512.0f*5, 0.0f);
	ground->rot_x(PAI * 0.5f);
	*/
	//---------------------------
	//リザルトマスター
	//---------------------------
	ResultMaster::Create( &m_resultMaster,m_device,m_objectList,m_updateList,m_drawListManager,m_import,m_fbxTexImport,m_debugproc,m_padXManager,m_light );

	return true;
}

// EOF
