//*****************************************************************************
//
// �^�C�g���t�F�[�Y [title.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "title.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\game\game.h"

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

#include "..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"
#include "..\..\manager\effectManager\effectManager.h"

#include "..\..\objectBase\fbxModel\fbxModel.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 50.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 100.0f, -250.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Title::Title(LPDIRECT3DDEVICE9 device) : Phase(device)
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

	m_effectManager		= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Title::~Title(void)
{
}

//=============================================================================
// ������
//=============================================================================
bool Title::Initialize(void)
{
	//----------------------------
	// �C���|�[�g
	//----------------------------
	if(!TitleImport::Create(&m_import, m_device))
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
	//m_fade = CFade::Create(m_device);
	//m_fade->Start(CFade::FADESTATE_IN, 1, 1.0f, 1.0f, 1.0f, 1.0f);

	//----------------------------
	// �X�e�[�^�X������
	//----------------------------

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Title::Finalize(void)
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

	//�G�t�F�N�g�}�l�[�W��
	SafeFinalizeDelete(m_effectManager);

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
void Title::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===�^�C�g���t�F�[�Y===\n");
#endif

	//----------------------------
	// �X�V���e
	//----------------------------
	PadX* pad = m_padXManager->pad(0);

	//----------------------------
	// �I�u�W�F�N�g�X�V
	//----------------------------
	m_updateList->AllUpdate();

	m_effectManager->Update();

	//----------------------------
	// ��ʑJ��
	//----------------------------

	if( m_keyboard->trigger( DIK_1 ) )
	{
		//m_effectManager->AddEffectFromDataBase( 0,D3DXVECTOR3(0,0,0) );
		m_effectManager->AddEffectFromDataBase( 1,D3DXVECTOR3(0,0,0) );
	}

	if(pad->buttonTrigger(XINPUT_GAMEPAD_A))
	{
		Manager::nextPhase((Phase*)new Game(m_device));
	}
}

//=============================================================================
// �`��
//=============================================================================
void Title::Draw(void)
{
	//----------------------------
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	//----------------------------
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(128, 64, 64, 255), 1.0f, 0);

	//----------------------------
	// �I�u�W�F�N�g�`��
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
bool Title::InitObject(void)
{
	//------------------------------
	//�G�t�F�N�g�@�������e�X�g
	//------------------------------
	InstancingBillboard *bill;
	if( !InstancingBillboard::Create( &bill,m_device,m_objectList,1,ObjectBase::TYPE_NONE,5000,
		"../resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1)))
		return false;
	m_drawListManager->Link( bill,1,Shader::PAT_INS );

	if( !EffectManager::Create( &m_effectManager,bill ) )
		return false;
	m_effectManager->LoadEffectData( "../resources/effect/FireWorks3.OEF" );
	m_effectManager->LoadEffectData( "../resources/effect/Chino.OEF" );
	m_effectManager->SetOption( InstancingBillboard::OPTION(true,false,false));

	//fbx
	FbxModel *fbx;
	if( !FbxModel::Create( &fbx,m_device,m_objectList,0,ObjectBase::TYPE_NONE,"../resources/fbxmodel/ggy.bin" ) )
		return false;

	m_drawListManager->Link( fbx,0,Shader::PAT_FBX );
	m_updateList->Link( fbx );
	fbx->StartAnimation(61,91,true );

	return true;
}

// EOF
