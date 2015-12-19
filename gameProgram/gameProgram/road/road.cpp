//*****************************************************************************
//
// ソースのテンプレート [road.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "road.h"

#include "..\thread\thread.h"
#include "roadmanager\roadmanager.h"

#include "..\phase\phase.h"
#include "..\import\fbx\fbxTexImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 性的変数定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RoadManager*			Road::m_roadManager;
bool					Road::m_roadingFlag;
bool					Road::m_primUpdate;
bool					Road::m_closeFlag;
Road::ROAD_STATE		Road::m_roadState;
Phase*					Road::m_initialize;
Phase*					Road::m_finalize;
//=============================================================================
// コンストラクタ
//=============================================================================
Road::Road(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_roadingFlag = false;
	m_roadState = ROAD_STATE_PRIM;
	m_initialize = nullptr;
	m_finalize = nullptr;
	m_primUpdate = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
Road::~Road(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Road::Create(Road** outPointer , LPDIRECT3DDEVICE9 device)
{
	Road* pointer = new Road();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;

	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Road::Initialize( LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// コメント
	//----------------------------
	m_thread = new Thread;
	RoadManager::Create( &m_roadManager , device );
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Road::Finalize(void)
{
	m_roadManager->Finalize();
	m_roadManager = nullptr;
	delete m_thread;
	m_thread = NULL;
	delete this;
}
//=============================================================================
// 襖を開ける(フェードアウト)
//=============================================================================
void Road::StateOpen( void )
{
	m_roadManager->StateOpen();
}
//=============================================================================
// 襖を閉じる(フェードイン)
//=============================================================================
void Road::StateClose( void )
{
	m_roadManager->StateClose();
}
//=============================================================================
// ロード画面
//=============================================================================
void Road::Roading( Phase* initialize , FbxTexImport* fbxImport)
{


	m_initialize = initialize;
	m_finalize = nullptr;
	m_thread->Create( &Change1 );
	m_roadingFlag = true;
	m_primUpdate = true;
	m_roadManager->InitializeClosePos();
	StateClose();
	while( m_roadingFlag )
	{
			m_roadManager->Update();
			if( m_roadState == ROAD_STATE_INITIALIZE_END || m_roadState == ROAD_STATE_COMPLETE )
			{
				if( m_primUpdate )
				{
					initialize->Update();
					m_primUpdate = false;
				}
				m_roadManager->Draw( initialize );
			}
			else
			{
				m_roadManager->Draw();
			}
			if( m_roadManager->GetState() == RoadManager::ROAD_STATE_NONE )
			{
				m_roadingFlag = false;
			}
	}

	m_thread->threadend();
}
void Road::Roading(Phase* finalize , Phase* initialize )
{
	m_finalize = finalize;
	m_initialize = initialize;
	m_thread->Create( &Change2 );
	m_roadingFlag = true;
	m_roadManager->SetState( RoadManager::ROAD_STATE_CLOSE );
	m_primUpdate = true;
	m_roadState = ROAD_STATE_PRIM;
	m_roadManager->InitializeOpenPos();
	while( m_roadingFlag )
	{
		m_roadManager->Update();
		if(m_roadManager->GetCloseFlag() == false )
		{
			m_roadManager->Draw( m_finalize );
		}
		else if(  m_roadState == ROAD_STATE_COMPLETE )
		{
			//if( m_primUpdate )
			//{
			//	initialize->Update();
			//	m_primUpdate = false;
			//}
			m_roadManager->Draw(m_initialize);

		}
		else
		{
			m_roadManager->Draw();
		}

		if( m_roadManager->GetState() == RoadManager::ROAD_STATE_NONE )
		{
			m_roadingFlag = false;
		}
	}
	m_thread->threadend();
}
//=============================================================================
// ロード本体
//=============================================================================
void Road::Change( void )
{
	//if( m_finalize == nullptr )
	//{
	//	Change1( m_initialize );
	//}
	//else
	//{
	//	Change2( m_finalize , m_initialize );
	//}
}
void Road::Change1( void )
{
	m_roadState = ROAD_STATE_FINALIZE_END;
	m_roadManager->InitializeClosePos();
	m_roadState = ROAD_STATE_INITIALIZE_START;
	m_initialize->Initialize();
	m_roadState = ROAD_STATE_INITIALIZE_END;
	StateOpen();
	//m_initialize = nullptr;
	//m_finalize = nullptr;
	m_roadState = ROAD_STATE_COMPLETE;
}
void Road::Change2( void )
{
	m_roadState = ROAD_STATE_PRIM;
	m_roadManager->InitializeOpenPos();
	StateClose();
	while( m_roadManager->GetCloseFlag() == false )
		Sleep(10);
	m_roadState = ROAD_STATE_FINALIZE_START;
	m_finalize->Finalize();
	delete m_finalize;
	m_roadState = ROAD_STATE_FINALIZE_END;
	m_roadState = ROAD_STATE_INITIALIZE_START;
	m_initialize->Initialize();
	m_roadState = ROAD_STATE_INITIALIZE_END;
	StateOpen();
	//m_initialize = nullptr;
	//m_finalize = nullptr;
	m_roadState = ROAD_STATE_COMPLETE;
}
// EOF
