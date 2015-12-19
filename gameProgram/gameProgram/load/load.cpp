//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [load.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
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
// ���I�ϐ���`
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
// �R���X�g���N�^
//=============================================================================
Load::Load(void)
{
	//----------------------------
	// �����o�[������
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
// �f�X�g���N�^
//=============================================================================
Load::~Load(void)
{
}

//=============================================================================
// ����
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
// ������
//=============================================================================
bool Load::Initialize( LPDIRECT3DDEVICE9 device )
{
	//----------------------------
	// �󂯎��
	//----------------------------
	m_device = device;

	//----------------------------
	// �X���b�h������
	//----------------------------
	m_thread = new Thread;
	LoadManager::Create( &m_loadManager , device );

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Load::Finalize(void)
{
	SafeFinalizeDelete(m_loadManager);
	SafeDelete(m_thread);
}
//=============================================================================
// �����J����(�t�F�[�h�A�E�g)
//=============================================================================
void Load::StateOpen( void )
{
	m_loadManager->StateOpen();
}
//=============================================================================
// �������(�t�F�[�h�C��)
//=============================================================================
void Load::StateClose( void )
{
	m_loadManager->StateClose();
}
//=============================================================================
// �N�����[�h���
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
// ���[�h�{��
//=============================================================================
void Load::SetUp( void )
{
	m_loadState = LOAD_STATE_FINALIZE_END;
	m_loadManager->InitializeClosePos();

	m_loadState = LOAD_STATE_INITIALIZE_START;

	//----------------------------
	// ���C���C���|�[�g
	//----------------------------
	if(!MainImport::Create(m_startLoad->mainImport, m_device))
		MessageBox(nullptr, "MainImport�������s","MainImport::Create",MB_OK);

	//----------------------------
	// fbx�e�N�X�`��
	//----------------------------
	if(!FbxTexImport::Create(m_startLoad->fbxTexImport, m_device))
		MessageBox(nullptr, "FbxTexImport�������s","FbxTexImport::Create",MB_OK);

	//----------------------------
	// �t�F�[�Y
	//----------------------------
	if(!m_startLoad->phase->Initialize())
		MessageBox(nullptr, "�t�F�[�Y���������s","m_initialize->Initialize()",MB_OK);

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
		MessageBox(nullptr, "�t�F�[�Y���������s","m_initialize->Initialize()",MB_OK);

	m_loadState = LOAD_STATE_INITIALIZE_END;
	StateOpen();

	m_loadState = LOAD_STATE_COMPLETE;
}
// EOF
