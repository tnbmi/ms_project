//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [road.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "road.h"
#include "..\thread\thread.h"
#include"roadmanager\roadmanager.h"
#include "..\phase\phase.h"
#include "..\manager\manager.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ���I�ϐ���`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RoadManager*			Road::m_roadManager;
bool					Road::m_roadingFlag;
bool					Road::m_primUpdate;
bool					Road::m_closeFlag;
Road::ROAD_STATE		Road::m_roadState;
Phase*					Road::m_initialize;
Phase*					Road::m_finalize;
Manager*				Road::m_manager;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
Road::Road(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_roadingFlag = false;
	m_roadState = ROAD_STATE_PRIM;
	m_initialize = nullptr;
	m_finalize = nullptr;
	m_primUpdate = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Road::~Road(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Road::Create(Road** outPointer , LPDIRECT3DDEVICE9 device , Manager *manager)
{
	Road* pointer = new Road();
	if(!pointer->Initialize(device , manager ))
		return false;

	*outPointer = pointer;

	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Road::Initialize( LPDIRECT3DDEVICE9 device , Manager *manager)
{
	//----------------------------
	// �R�����g
	//----------------------------
	m_thread = new Thread;
	m_manager = manager;
	RoadManager::Create( &m_roadManager , device );
	return true;
}

//=============================================================================
// �I��
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
// �����J����(�t�F�[�h�A�E�g)
//=============================================================================
void Road::StateOpen( void )
{
	m_roadManager->StateOpen();
}
//=============================================================================
// �������(�t�F�[�h�C��)
//=============================================================================
void Road::StateClose( void )
{
	m_roadManager->StateClose();
}
//=============================================================================
// ���[�h���
//=============================================================================
void Road::Roading( Phase* initialize )
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
// ���[�h�{��
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
	{
	}
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
