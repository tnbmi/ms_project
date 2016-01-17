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
#include "..\..\tex2DAnimation\tex2DAnimation.h"

#include "..\..\effectManager\effectManager.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"

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

	m_timerCnt = 60 * 30;
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
	//アニメーションシステム
	//----------------------------
	SafeFinalizeDelete( m_pushLogo );

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

	//エフェクト
	SafeFinalizeDelete( m_effectManager );

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
	//アニメーション更新
	//----------------------------
	m_pushLogo->Update();

	//----------------------------
	// オブジェクト更新
	//----------------------------
	m_updateList->AllUpdate();

	m_effectManager->Update();
	
	//テーブル更新
	if( !m_nebRed->IsPlayAnim() )
	{
		m_animIdx++;

		if( m_animIdx >= 8 )
		{
			m_animIdx = 0;
		}

		m_nebRed->StartAnimation( m_animTable[m_animIdx].nebSt,m_animTable[m_animIdx].nebEd,false );
		m_nebBlue->StartAnimation( m_animTable[m_animIdx].nebSt,m_animTable[m_animIdx].nebEd,false );

	}

	//----------------------------
	// 画面遷移
	//----------------------------
	// カウント
	m_timerCnt--;

	if(m_padXManager->InputChk(0xf03f) || m_keyboard->trigger(DIK_RETURN))
	{
		Manager::nextPhase((Phase*)new Standby(m_device));
		Sound::Play( Sound::SE_KAIMAKU );
	}

	if(m_padXManager->InputChk(0x0100) || m_timerCnt <= 0 || m_keyboard->trigger(DIK_0))
	{
		Manager::nextPhase((Phase*)new Demo(m_device));
		Sound::Play( Sound::SE_KAIMAKU );
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
	m_drawListManager->Link(dome, 0, Shader::PAT_NONE_LIGHT);
	dome->pos(0.0f, -150.0f, 1000.0f);
	dome->rot_y(PAI * 0.5f);

	//----------------------------
	// 背景3Dポリゴン
	//----------------------------
	Polygon3D* bg3D;
	if(!Polygon3D::Create(&bg3D, m_device, m_objectList, m_import->texture(TitleImport::BG)))
		return false;
	m_updateList->Link(bg3D);
	m_drawListManager->Link(bg3D, 1, Shader::PAT_NONE_LIGHT);
	D3DXVECTOR3 size = D3DXVECTOR3(540.0f*2.3f, 720.0f*2.3f, 0.0f);
	bg3D->scl(size);
	bg3D->pos(0.0f, 400.0f, 2500.0f);
	bg3D->texcoord(0, 0.0f, 0.01f);
	bg3D->texcoord(1, 0.3f, 0.01f);
	bg3D->texcoord(3, 0.3f, 1.0f);

	//------------------------------
	//プッシュロゴ
	//------------------------------
	Tex2DAnimation::Create(&m_pushLogo);

	Polygon2D *p;
	Polygon2D::Create( &p,m_device,m_objectList,m_import->texture( TitleImport::START_LOGO ),ObjectBase::TYPE_2D );
	
	m_updateList->Link(p);
	m_drawListManager->Link(p, 4, Shader::PAT_2D);
	
	

	m_pushLogo->Set2DPolygon(p);
	m_pushLogo->SetTexture( m_import->texture(TitleImport::START_LOGO) );
	m_pushLogo->SetAnimationData( 15,10,D3DXVECTOR2( 0.0f,0.0f),D3DXVECTOR2( 1.0f,0.0f),
									D3DXVECTOR2( 0.0f,0.1f),D3DXVECTOR2( 1.0f,0.1f),
									D3DXVECTOR2( 0.0f,0.1f) );
	m_pushLogo->StartAnimation(true);
	p->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT - SCREEN_HEIGHT/4,0);
	p->scl( 441,81,0 );

	//初期化対策
	m_pushLogo->Update();

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

	//じじーに合わせたアニメーションテーブル
	m_animTable[0].nebSt = 1;
	m_animTable[0].nebEd = 30;

	m_animTable[1].nebSt = 451;
	m_animTable[1].nebEd = 510;

	m_animTable[2].nebSt = 511;
	m_animTable[2].nebEd = 570;

	m_animTable[3].nebSt = 31;
	m_animTable[3].nebEd = 60;

	m_animTable[4].nebSt = 91;
	m_animTable[4].nebEd = 120;

	m_animTable[5].nebSt = 151;
	m_animTable[5].nebEd = 180;

	m_animTable[6].nebSt = 211;
	m_animTable[6].nebEd = 240;

	m_animTable[7].nebSt = 271;
	m_animTable[7].nebEd = 330;

	m_animIdx = 0;

	FbxModel *nebBlue;
	FbxModel::Create( &nebBlue,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/nebta_blue.bin",m_fbxTexImport );
	nebBlue->StartAnimation( m_animTable[0].nebSt,m_animTable[0].nebEd,false );
	m_updateList->Link( nebBlue );
	m_drawListManager->Link( nebBlue,0,Shader::PAT_FBX );

	nebBlue->pos( D3DXVECTOR3( -1000,0,1300 ) );
	nebBlue->rot( D3DXVECTOR3(0,3*PAI /4,0 ) );

	FbxModel *nebRed;
	FbxModel::Create( &nebRed,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/nebta_red.bin",m_fbxTexImport );
	nebRed->StartAnimation(  m_animTable[0].nebSt,m_animTable[0].nebEd,false );
	m_updateList->Link( nebRed );
	m_drawListManager->Link( nebRed,0,Shader::PAT_FBX );

	nebRed->pos( D3DXVECTOR3( 1000,0,1300 ) );
	nebRed->rot( D3DXVECTOR3(0,-3*PAI /4,0 ) );

	//代入
	m_nebRed = nebRed;
	m_nebBlue= nebBlue;


	//ggy
	//一通りアニメーション
	FbxModel *ggyRed,*ggyBlue;
	FbxModel::Create( &ggyRed,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy_red_title.bin",m_fbxTexImport );
	ggyRed->pos( D3DXVECTOR3( 200,0,0 ) );
	ggyRed->rot( D3DXVECTOR3(0,-3*PAI /4,0 ) );
	ggyRed->StartAnimation( 1,330,true );
	m_updateList->Link( ggyRed );
	m_drawListManager->Link( ggyRed,0,Shader::PAT_FBX );

	FbxModel::Create( &ggyBlue,m_device,m_objectList,0,ObjectBase::TYPE_3D,"./resources/fbxModel/ggy_blue_title.bin",m_fbxTexImport );
	ggyBlue->pos( D3DXVECTOR3( -200,0,0 ) );
	ggyBlue->rot( D3DXVECTOR3(0,3*PAI /4,0 ) );
	ggyBlue->StartAnimation( 1,330,true );
	m_updateList->Link( ggyBlue );
	m_drawListManager->Link( ggyBlue,0,Shader::PAT_FBX );


	//初期化対策
	nebRed->Update();
	nebBlue->Update();
	ggyRed->Update();
	ggyBlue->Update();
	fbx->Update();
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

	//----------------------------
	//エフェクトマネージャ
	//----------------------------
	EffectManager::Create( &m_effectManager,m_device,m_objectList,m_updateList,m_drawListManager,3000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_TeamBlue.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_TeamRed.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_Title1.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks_Title2.OEF" );	

	m_effectManager->SetOption( InstancingBillboard::OPTION( true,false,false ) );

	m_effectManager->SetEffectGenData( 0,120,0,D3DXVECTOR3( -500,850,2500 ) );
	m_effectManager->SetEffectGenData( 1,120,1,D3DXVECTOR3( 500,850,2500 ) );

	m_effectManager->SetEffectGenData( 2,240,2,D3DXVECTOR3( -1200,1250,1300 ) );
	m_effectManager->SetEffectGenData( 3,240,3,D3DXVECTOR3( 1200,1250,1300 ) );

	return true;
}

// EOF
