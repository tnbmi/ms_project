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
#include"roadmanager\roadmanager.h"
#include "..\phase\phase.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 性的変数定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RoadManager*			Road::m_roadManager;
bool					Road::m_roadingFlag;
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
bool Road::Create(Road** outPointer , LPDIRECT3DDEVICE9 device )
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
bool Road::Initialize( LPDIRECT3DDEVICE9 device )
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
void Road::Roading( Phase* initialize )
{
	m_initialize = initialize;
	m_finalize = nullptr;
	m_thread->Create( &Change );
	m_roadingFlag = true;
	StateClose();

	while( m_roadingFlag )
	{
		m_roadManager->Update();
		m_roadManager->Draw();
		if( m_roadState == ROAD_STATE_INITIALIZE_END )
		{
			initialize->Draw();
		}
		if( m_roadManager->GetState() == RoadManager::ROAD_STATE_NONE )
		{
			m_roadingFlag = false;
		}
	}
}
void Road::Roading(Phase* finalize , Phase* initialize )
{
	m_finalize = finalize;
	m_initialize = initialize;
	m_thread->Create( &Change );
	m_roadingFlag = true;

	while( m_roadingFlag )
	{
		if( m_roadState == ROAD_STATE_PRIM )
		{
			finalize->Draw();
		}
		if( m_roadState == ROAD_STATE_INITIALIZE_END )
		{
			initialize->Draw();
		}
		m_roadManager->Update();
		m_roadManager->Draw();
		if( m_roadManager->GetState() == RoadManager::ROAD_STATE_NONE )
		{
			m_roadingFlag = false;
		}
	}
}
//=============================================================================
// ロード本体
//=============================================================================
void Road::Change( void )
{
	if( m_finalize == nullptr )
	{
		Change1( m_initialize );
	}
	else
	{
		Change2( m_finalize , m_initialize );
	}
}
void Road::Change1( Phase* initialize )
{
	m_roadState = ROAD_STATE_FINALIZE_END;
	m_roadManager->InitializeClosePos();
	m_roadState = ROAD_STATE_INITIALIZE_START;
	initialize->Initialize();
	m_roadState = ROAD_STATE_INITIALIZE_END;
	StateOpen();
	m_roadState = ROAD_STATE_COMPLETE;
	initialize = nullptr;
}
void Road::Change2(Phase* finalize , Phase* initialize )
{
	m_roadState = ROAD_STATE_PRIM;
	StateClose();
	m_roadState = ROAD_STATE_FINALIZE_START;
	finalize->Finalize();
	m_roadState = ROAD_STATE_FINALIZE_END;
	m_roadState = ROAD_STATE_INITIALIZE_START;
	initialize->Initialize();
	m_roadState = ROAD_STATE_INITIALIZE_END;
	StateOpen();
	m_roadState = ROAD_STATE_COMPLETE;
	initialize = nullptr;
	finalize = nullptr;
}
// EOF
