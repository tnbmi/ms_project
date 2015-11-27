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
#include "..\result\result.h"

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

#include "..\..\commandmanager\commandmanager.h"

#include "..\..\manager\effectManager\effectManager.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"
#include "..\..\object\player\player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\input\keyboard\keyboard.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 150.0f, -1000.0f);

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

	//�G�t�F�N�g
	m_effectManager->Finalize();
	SafeFinalizeDelete( m_effectManager );

	//�v���C���[�폜
	m_redTeam->Finalize();
	m_blueTeam->Finalize();
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );

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
	//Player�X�V
	//----------------------------
	m_redTeam->Update();
	m_blueTeam->Update();

	//----------------------------
	//EffectManager�X�V
	//----------------------------
	m_effectManager->Update();

	if( m_keyboard->trigger(DIK_1 ) )
	{
		m_effectManager->AddEffectFromDataBase(0,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_2 ) )
	{
		m_effectManager->AddEffectFromDataBase(1,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_3 ) )
	{
		m_effectManager->AddEffectFromDataBase(2,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_4 ) )
	{
		m_effectManager->AddEffectFromDataBase(3,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_5 ) )
	{
		m_effectManager->AddEffectFromDataBase(4,D3DXVECTOR3(0,0,0) );
	}

	if( m_keyboard->trigger(DIK_6 ) )
	{
		m_effectManager->AddEffectFromDataBase(5,D3DXVECTOR3(0,60,0) );
	}

	if( m_keyboard->trigger(DIK_7 ) )
	{
		m_effectManager->AddEffectFromDataBase(6,D3DXVECTOR3(0,60,0) );
	}

	if( m_keyboard->trigger( DIK_0 ) )
	{
		m_redTeam->Move( D3DXVECTOR3( -500,0,0 ),D3DXVECTOR3(-30,0,0),500 );
		m_blueTeam->Move( D3DXVECTOR3( 500,0,0 ),D3DXVECTOR3(30,0,0),500 );
	}

	//----------------------------
	// ��ʑJ��
	//----------------------------
	if(m_command_manager->GetState() == Commandmanager::TEAM0_WIN || 
	   m_command_manager->GetState() == Commandmanager::TEAM1_WIN )
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
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
bool Game::InitObject(void)
{
	//----------------------------
	// �R�}���h�}�l�[�W������
	//----------------------------
	Commandmanager::Create(&m_command_manager, m_padXManager, m_debugproc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);

	//----------------------------
	// 3D�|���S���e�X�g
	//----------------------------
	Polygon3D* poly3d;
	if(!Polygon3D::Create(&poly3d, m_device, m_objectList, m_import->texture(GameImport::TEST_0)))
		return false;
	m_updateList->Link(poly3d);
	m_drawListManager->Link(poly3d, 4, Shader::PAT_LIGHT);
	poly3d->scl(512.0f, 512.0f, 0.0f);
	poly3d->rot_x(PAI * 0.5f);

	//----------------------------
	//�G�t�F�N�g�}�l�[�W������
	//----------------------------
	InstancingBillboard *bill;
	InstancingBillboard::Create(&bill,m_device,m_objectList,1,ObjectBase::TYPE_3D,10000,"../resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1));
	m_drawListManager->Link( bill,0,Shader::PAT_INS );
	m_updateList->Link( bill );
	EffectManager::Create( &m_effectManager,bill );
	m_effectManager->SetOption( InstancingBillboard::OPTION(true,false,false ) );
	//�G�t�F�N�g���[�h����񂲂�����������������������
	m_effectManager->LoadEffectData("../resources/effect/Chino.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Cocoa.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Rize.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Syaro.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya2.OEF" );
	m_effectManager->LoadEffectData("../resources/effect/Chiya3.OEF" );
	

	//----------------------------
	//�v���C���[����
	//----------------------------
	Player::Create( &m_blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin","../resources/fbxModel/ggy.bin");
	Player::Create( &m_redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin","../resources/fbxModel/ggy.bin");
	m_redTeam->rot( D3DXVECTOR3(0,D3DX_PI/2,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,-D3DX_PI/2,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,150,0 ) );
	m_blueTeam->offsetPos( D3DXVECTOR3(0,150,0 ) );

	m_redTeam->Move( D3DXVECTOR3(-500,0,0),D3DXVECTOR3(-40,0,0),300 );
	m_blueTeam->Move( D3DXVECTOR3(500,0,0),D3DXVECTOR3(40,0,0),300 );

	//---------------------------
	//
	//---------------------------

	for( int i = 0 ; i < 5 ; i++ )
	{
		FbxModel *fbx;
		FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/ggy.bin" );
		m_updateList->Link( fbx );
		m_drawListManager->Link( fbx,0,Shader::PAT_FBX );

		fbx->pos( D3DXVECTOR3( -600,0,100+100 * i ) );
	}

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
