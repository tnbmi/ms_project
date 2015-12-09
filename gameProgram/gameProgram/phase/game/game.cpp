//*****************************************************************************
//
// �Q�[���t�F�[�Y [game.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "game.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\..\sound\sound.h"
#include "..\result\result.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\game\gameImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\objectBase\polygon3D\polygon3D.h"
#include "..\..\objectBase\meshDome\meshDome.h"

#include "..\..\commandmanager\commandmanager.h"
#include "..\..\timemanager\timeManager.h"

#include "..\..\phase\game\gameMaster\gameMaster.h"
#include "..\..\object\player\player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 500.0f, 10000.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 300.0f, -2000.0f);
const int _time_max		= 3000;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Game::Game(LPDIRECT3DDEVICE9 device) : Phase(device)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_import = nullptr;

	m_shader = nullptr;
	m_camera = nullptr;
	m_light	 = nullptr;

	m_objectList		= nullptr;
	m_updateList		= nullptr;
	m_drawListManager	= nullptr;

	m_command_manager	= nullptr;
	m_time_manager		= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Game::~Game(void)
{
}

//=============================================================================
// ������
//=============================================================================
bool Game::Initialize(void)
{
	//----------------------------
	// �C���|�[�g
	//----------------------------
	if(!GameImport::Create(&m_import, m_device))
		return false;

	//----------------------------
	// �V�F�[�_�[
	//----------------------------
	if(!Shader::Create(&m_shader, m_device))
		return false;

	//----------------------------
	// �r���[
	//----------------------------
	// �J����
	if(!Camera::Create(&m_camera, m_device, _at, _eye))
		return false;

	// ���C�g
	if(!Light::Create(&m_light, m_device))
		return false;
	m_light->dirLightVector(-1.0f, -3.0f, 2.0f);

	//----------------------------
	// �Ǘ����X�g
	//----------------------------
	// �I�u�W�F�N�g���X�g
	if(!ObjectList::Create(&m_objectList))
		return false;

	// �X�V���X�g
	if(!UpdateList::Create(&m_updateList))
		return false;

	// �`�惊�X�g
	if(!DrawListManager::Create(&m_drawListManager, m_device, m_shader))
		return false;

	//----------------------------
	// �I�u�W�F�N�g
	//----------------------------
	if(!InitObject())
		return false;

	//----------------------------
	// �T�E���h�̍Đ�
	//----------------------------
	Sound::Play(Sound::BGM_A);

	//----------------------------
	// �t�F�[�h
	//----------------------------
	//m_fade = CFade::Create(device);
	//m_fade->Start(CFade::FADESTATE_IN, 1, 1.0f, 1.0f, 1.0f, 1.0f);

	//----------------------------
	// �X�e�[�^�X������
	//----------------------------

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Game::Finalize(void)
{
	//----------------------------
	// �I�u�W�F�N�g
	//----------------------------
	// ���݂���I�u�W�F�N�g�S�č폜
	m_objectList->AllDarelete();

	// �`�惊�X�g�}�l�[�W���[
	SafeFinalizeDelete(m_drawListManager);

	// �X�V���X�g
	SafeDelete(m_updateList);

	// �I�u�W�F�N�g���X�g
	SafeDelete(m_objectList);

	// �R�}���h
	SafeFinalizeDelete(m_command_manager);

	// �^�C��
	SafeFinalizeDelete(m_time_manager);

	//�Q�[���}�X�^�[
	SafeFinalizeDelete( m_gameMaster );

	//----------------------------
	// �r���[
	//----------------------------
	// �J����
	SafeFinalizeDelete(m_camera);

	// ���C�g
	SafeFinalizeDelete(m_light);

	//----------------------------
	// �V�F�[�_�[
	//----------------------------
	SafeFinalizeDelete(m_shader);

	//----------------------------
	// �C���|�[�g
	//----------------------------
	SafeFinalizeDelete(m_import);

	//----------------------------
	// �T�E���h�̒�~
	//----------------------------
	Sound::StopAll();
}

//=============================================================================
// �X�V
//=============================================================================
void Game::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===�Q�[���t�F�[�Y===\n");
#endif

	m_camera->SetCamera();
	//----------------------------
	// �X�V���e
	//----------------------------
	PadX* pad = m_padXManager->pad(0);

	//----------------------------
	// �I�u�W�F�N�g�X�V
	//----------------------------
	m_updateList->AllUpdate();

	//----------------------------
	// �R�}���h�}�l�[�W��
	//----------------------------
	m_command_manager->Update();

	//----------------------------
	// �^�C���}�l�[�W��
	//----------------------------
	bool transition = m_time_manager->Update();

	//transition = 1;

	//----------------------------
	//GameMaster�X�V
	//----------------------------
	m_gameMaster->Update();

	//----------------------------
	// ��ʑJ��
	//----------------------------				//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	if(transition)								// ����^�C���̂݁A�`�[���ł̏��s��(�ꉞ)�ǉ��\��
	{
		Manager::nextPhase((Phase*)new Result(m_device));
	}
	
}

//=============================================================================
// �`��
//=============================================================================
void Game::Draw(void)
{
	//----------------------------
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	//----------------------------
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(64, 64, 128, 256), 1.0f, 0);

	//----------------------------
	// �I�u�W�F�N�g�`��
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);

	//----------------------------
	// �R�}���h�}�l�[�W���`��
	//----------------------------
	m_command_manager->Draw();

	//----------------------------
	// �^�C���}�l�[�W���`��
	//----------------------------
	m_time_manager->Draw();
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
bool Game::InitObject(void)
{
	//----------------------------
	// �R�}���h�}�l�[�W������
	//----------------------------
	CommandManager::Create(&m_command_manager, m_padXManager, m_debugproc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);

	//----------------------------
	// �^�C���}�l�[�W������
	//----------------------------
	TimeManager::Create(&m_time_manager, m_objectList, m_updateList, m_drawListManager, m_device, m_import, _time_max);

	//----------------------------
	// ���b�V���h�[���e�X�g
	//----------------------------
	MeshDome* dome;
	if(!MeshDome::Create(&dome, m_device, m_objectList,
		D3DXVECTOR2(10, 10), 3000.0f, 2000.0f, m_import->texture(GameImport::SKY)))
		return false;
	m_updateList->Link(dome);
	m_drawListManager->Link(dome, 2, Shader::PAT_LIGHT);

	//----------------------------
	// 3D�|���S���e�X�g
	//----------------------------
	Polygon3D* poly3d;
	if(!Polygon3D::Create(&poly3d, m_device, m_objectList, m_import->texture(GameImport::TEST_0)))
		return false;
	m_updateList->Link(poly3d);
	m_drawListManager->Link(poly3d, 4, Shader::PAT_LIGHT);
	poly3d->scl(512.0f*5, 512.0f*5, 0.0f);
	poly3d->rot_x(PAI * 0.5f);
	
	//----------------------------
	//�Q�[���}�X�^�[����
	//----------------------------
	GameMaster::Create( &m_gameMaster,m_device,m_objectList,m_updateList,m_drawListManager,m_import,m_debugproc,m_padXManager);

	/*
	for( int i = 0 ; i < 5 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( 600,0,100+100 * i ) );
	}

	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,500 ) );
	}
	
	for( int i = 0 ; i < 7 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,400 ) );
	}
	
	/*
	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,600 ) );
	}
	
	
	for( int i = 0 ; i < 10 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -550 + 100 * i,0,700 ) );
	}
	*/
	return true;
}

// EOF
