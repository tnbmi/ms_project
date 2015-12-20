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
#include "..\..\sound\sound.h"
#include "..\standby\standby.h"
#include "..\demo\demo.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\title\titleImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon3D\polygon3D.h"
#include "..\..\objectBase\meshDome\meshDome.h"
#include "..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"

#include "..\..\import\fbx\fbxTexImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 500.0f, 800.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 100.0f, -770.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
Title::Title(LPDIRECT3DDEVICE9 device) : Phase(device)
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
	m_fbxTexImport		= nullptr;
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
	// インポート
	//----------------------------
	if(!TitleImport::Create(&m_import, m_device))
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
	Sound::Play(Sound::BGM_TITLE03);

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
void Title::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===タイトルフェーズ===\n");
#endif

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	//----------------------------
	// 画面遷移
	//----------------------------
	if(m_padXManager->InputChk(0xf03f) || m_keyboard->trigger(DIK_RETURN))
		Manager::nextPhase((Phase*)new Standby(m_device));

	if(m_padXManager->InputChk(0x0100) || m_keyboard->trigger(DIK_0))
		Manager::nextPhase((Phase*)new Demo(m_device));
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
					D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//----------------------------
	// オブジェクト描画
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool Title::InitObject(void)
{
	//----------------------------
	// 空メッシュドーム
	//----------------------------
	MeshDome* dome;
	if(!MeshDome::Create(&dome, m_device, m_objectList,
		D3DXVECTOR2(8, 3), 2500.0f, 2000.0f, m_import->texture(TitleImport::SKY)))
		return false;
	m_updateList->Link(dome);
	m_drawListManager->Link(dome, 2, Shader::PAT_NONE_LIGHT);
	dome->pos(0.0f, -150.0f, 1000.0f);
	dome->rot_y(PAI * 0.5f);

	//----------------------------
	// 背景3Dポリゴン
	//----------------------------
/*	Polygon3D* bg3D;
	if(!Polygon3D::Create(&bg3D, m_device, m_objectList, m_import->texture(TitleImport::BG)))
		return false;
	m_updateList->Link(bg3D);
	m_drawListManager->Link(bg3D, 1, Shader::PAT_NONE_LIGHT);
	D3DXVECTOR3 size = D3DXVECTOR3(1800.0f, 720.0f, 0.0f);
	bg3D->scl(size);
	bg3D->pos(0.0f, size.y*0.5f, 512.0f*2.5f);
	bg3D->texcoord(0, 0.0f, 0.01f);
	bg3D->texcoord(1, 1.0f, 0.01f);
*/
	//------------------------------
	//fbx
	//------------------------------
	FbxModel *fbx;
	FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/title.bin",m_fbxTexImport );
	fbx->StartAnimation( 1,3,true );
	m_updateList->Link( fbx );
	m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

	fbx->pos( D3DXVECTOR3( -0,0,0 ) );
	fbx->rot( D3DXVECTOR3(0,PAI,0 ) );

	FbxModel *nebBlue;
	FbxModel::Create( &nebBlue,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/nebta_blue.bin",m_fbxTexImport );
	nebBlue->StartAnimation( 1,30,true );
	m_updateList->Link( nebBlue );
	m_drawListManager->Link( nebBlue,0,Shader::PAT_FBX );

	nebBlue->pos( D3DXVECTOR3( -1000,0,1300 ) );
	nebBlue->rot( D3DXVECTOR3(0,3*PAI /4,0 ) );

	FbxModel *nebRed;
	FbxModel::Create( &nebRed,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/nebta_red.bin",m_fbxTexImport );
	nebRed->StartAnimation( 1,30,true );
	m_updateList->Link( nebRed );
	m_drawListManager->Link( nebRed,0,Shader::PAT_FBX );

	nebRed->pos( D3DXVECTOR3( 1000,0,1300 ) );
	nebRed->rot( D3DXVECTOR3(0,-3*PAI /4,0 ) );

	//----------------------------
	// タイトルロゴ
	//----------------------------
	Polygon2D* poly2d;
	if(!Polygon2D::Create(&poly2d, m_device, m_objectList, m_import->texture(TitleImport::TITLE_LOGO)))
		return false;
	m_updateList->Link(poly2d);
	m_drawListManager->Link(poly2d, 4, Shader::PAT_2D);
	poly2d->scl(701.0f, 248.0f, 0.0f);
	poly2d->pos(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f);

	return true;
}

// EOF
