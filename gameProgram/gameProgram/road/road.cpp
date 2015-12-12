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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ���I�ϐ���`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
RoadManager*			Road::m_roadManager;
bool					Road::m_roadingFlag;
Road::ROAD_STATE		Road::m_roadState;
Phase*					Road::m_initialize;
Phase*					Road::m_finalize;

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
bool Road::Create(Road** outPointer , LPDIRECT3DDEVICE9 device )
{
	Road* pointer = new Road();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;

	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Road::Initialize( LPDIRECT3DDEVICE9 device )
{
	//----------------------------
	// �R�����g
	//----------------------------
	m_thread = new Thread;
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
// ���[�h�{��
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
