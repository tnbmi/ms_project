//*****************************************************************************
//
// ソースのテンプレート [load.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "load.h"
#include "..\common\safe.h"

#include "..\thread\thread.h"
#include "loadmanager\loadmanager.h"

#include "..\phase\phase.h"
#include "..\sound\sound.h"
#include "..\import\main\mainImport.h"
#include "..\import\fbx\fbxTexImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 性的変数定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LPDIRECT3DDEVICE9	Load::m_device		 = nullptr;
LoadManager*		Load::m_loadManager	 = nullptr;

bool				Load::m_loadingFlag	 = false;
bool				Load::m_primUpdate	 = false;
bool				Load::m_closeFlag	 = false;
Load::LOAD_STATE	Load::m_loadState	 = LOAD_STATE_PRIM;

Phase* Load::m_initialize	= nullptr;
Phase* Load::m_finalize		= nullptr;

Load::START_LOAD* Load::m_startLoad = nullptr;

//=============================================================================
// コンストラクタ
//=============================================================================
Load::Load(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_device		= nullptr;
	m_loadManager	= nullptr;
	m_loadingFlag	= false;
	m_primUpdate	= false;
	m_closeFlag		= false;
	m_loadState		= LOAD_STATE_PRIM;
	m_initialize	= nullptr;
	m_finalize		= nullptr;
	m_startLoad		= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Load::~Load(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Load::Create(Load** outPointer , LPDIRECT3DDEVICE9 device )
{
	Load* pointer = new Load();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;

	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Load::Initialize( LPDIRECT3DDEVICE9 device )
{
	//----------------------------
	// 受け取り
	//----------------------------
	m_device = device;

	//----------------------------
	// スレッド初期化
	//----------------------------
	m_thread = new Thread;
	LoadManager::Create( &m_loadManager , device );

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Load::Finalize(void)
{
	SafeFinalizeDelete(m_loadManager);
	SafeDelete(m_thread);
}
//=============================================================================
// 襖を開ける(フェードアウト)
//=============================================================================
void Load::StateOpen( void )
{
	m_loadManager->StateOpen();
}
//=============================================================================
// 襖を閉じる(フェードイン)
//=============================================================================
void Load::StateClose( void )
{
	m_loadManager->StateClose();
}
//=============================================================================
// 起動ロード画面
//=============================================================================
void Load::StartLoading( START_LOAD* startLoad )
{
	m_startLoad = startLoad;

	m_thread->Create( &SetUp );
	m_loadingFlag = true;
	m_primUpdate = true;
	m_loadManager->InitializeClosePos();
	StateClose();

	while( m_loadingFlag )
	{
			m_loadManager->Update();
			if( m_loadState == LOAD_STATE_INITIALIZE_END || m_loadState == LOAD_STATE_COMPLETE )
			{
				if( m_primUpdate )
				{
					m_startLoad->phase->Update();
					m_primUpdate = false;
				}
				m_loadManager->Draw(m_startLoad->phase);
			}
			else
			{
				m_loadManager->Draw();
			}
			if( m_loadManager->GetState() == LoadManager::LOAD_STATE_NONE )
			{
				m_loadingFlag = false;
			}
	}

	m_thread->threadend();
}
void Load::Loading(Phase* finalize , Phase* initialize )
{
	m_finalize = finalize;
	m_initialize = initialize;

	m_thread->Create( &Change );
	m_loadingFlag = true;
	m_loadManager->SetState( LoadManager::LOAD_STATE_CLOSE );
	m_primUpdate = true;
	m_loadState = LOAD_STATE_PRIM;
	m_loadManager->InitializeOpenPos();

	while( m_loadingFlag )
	{
		m_loadManager->Update();
		if(m_loadManager->GetCloseFlag() == false )
		{
			m_loadManager->Draw( m_finalize );
		}
		else if(  m_loadState == LOAD_STATE_COMPLETE )
		{
			//if( m_primUpdate )
			//{
			//	initialize->Update();
			//	m_primUpdate = false;
			//}
			m_loadManager->Draw(m_initialize);

		}
		else
		{
			m_loadManager->Draw();
		}

		if( m_loadManager->GetState() == LoadManager::LOAD_STATE_NONE )
		{
			m_loadingFlag = false;
		}
	}
	m_thread->threadend();
}
//=============================================================================
// ロード本体
//=============================================================================
void Load::SetUp( void )
{
	m_loadState = LOAD_STATE_FINALIZE_END;
	m_loadManager->InitializeClosePos();

	m_loadState = LOAD_STATE_INITIALIZE_START;

	//----------------------------
	// メインインポート
	//----------------------------
	if(!MainImport::Create(m_startLoad->mainImport, m_device))
		MessageBox(nullptr, "MainImport生成失敗","MainImport::Create",MB_OK);

	//----------------------------
	// fbxテクスチャ
	//----------------------------
	if(!FbxTexImport::Create(m_startLoad->fbxTexImport, m_device))
		MessageBox(nullptr, "FbxTexImport生成失敗","FbxTexImport::Create",MB_OK);

	//----------------------------
	// フェーズ
	//----------------------------
	if(!m_startLoad->phase->Initialize())
		MessageBox(nullptr, "フェーズ初期化失敗","m_initialize->Initialize()",MB_OK);

	m_loadState = LOAD_STATE_INITIALIZE_END;
	StateOpen();

	m_loadState = LOAD_STATE_COMPLETE;
}

void Load::Change( void )
{
	m_loadState = LOAD_STATE_PRIM;
	m_loadManager->InitializeOpenPos();

	StateClose();
	while(!m_loadManager->GetCloseFlag())
		Sleep(10);

	m_loadState = LOAD_STATE_FINALIZE_START;
	SafeFinalizeDelete(m_finalize);

	m_loadState = LOAD_STATE_FINALIZE_END;
	m_loadState = LOAD_STATE_INITIALIZE_START;
	if(!m_initialize->Initialize())
		MessageBox(nullptr, "フェーズ初期化失敗","m_initialize->Initialize()",MB_OK);

	m_loadState = LOAD_STATE_INITIALIZE_END;
	StateOpen();

	m_loadState = LOAD_STATE_COMPLETE;
}
// EOF
