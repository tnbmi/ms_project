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
#include "..\demo\demo.h"

#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\title\titleImport.h"
#include "..\..\view\camera\camera.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\commandmanager\commandmanager.h"
#include "..\..\objectBase\polygon2D\polygon2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 0.0f, -50.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Title::Title(LPDIRECT3DDEVICE9 device) : Phase(device)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_import = nullptr;
	m_camera = nullptr;

	m_objectList		= nullptr;
	m_updateList		= nullptr;
	m_drawListManager	= nullptr;

	m_command_manager	= nullptr;
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
	// �r���[
	//----------------------------
	// �J����
	if(!Camera::Create(&m_camera, m_device, _at, _eye))
		return false;

	// ���C�g

	//----------------------------
	// �C���|�[�g
	//----------------------------
	if(!TitleImport::Create(&m_import, m_device))
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
	if(!DrawListManager::Create(&m_drawListManager, m_device))
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

	// �R�}���h
	SafeFinalizeDelete(m_command_manager);

	//----------------------------
	// �C���|�[�g
	//----------------------------
	SafeFinalizeDelete(m_import);

	//----------------------------
	// �r���[
	//----------------------------
	// �J����
	SafeFinalizeDelete(m_camera);

	// ���C�g

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

	m_command_manager->Update();

	//----------------------------
	// ��ʑJ��
	//----------------------------
	if(pad->buttonTrigger(XINPUT_GAMEPAD_A))
	{
	//	Manager::nextPhase((Phase*)new Demo(m_device));
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
					D3DCOLOR_RGBA(32, 0, 0, 255), 1.0f, 0);

	//----------------------------
	// 2D�`��
	//----------------------------
	m_drawListManager->AllDraw(m_camera->viewProjection());

	m_command_manager->Draw();
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
bool Title::InitObject(void)
{
	Commandmanager::Create(&m_command_manager, m_padXManager, m_debugproc);

	//----------------------------
	// 2D�|���S���e�X�g
	//----------------------------
	Polygon2D* poly2d;
	if(!Polygon2D::Create(&poly2d, m_device, m_objectList, m_import->texture(TitleImport::TEST_0)))
		return false;
	m_updateList->Link(poly2d);
	m_drawListManager->Link(poly2d, 4, Shader::PAT_2D);
	poly2d->pos(200.0f, 200.0f, 0.0f);
	poly2d->scl(128.0f, 128.0f, 0.0f);

	if(!Polygon2D::Create(&poly2d, m_device, m_objectList, m_import->texture(TitleImport::TEST_0)))
		return false;
	m_updateList->Link(poly2d);
	m_drawListManager->Link(poly2d, 4, Shader::PAT_2D);
	poly2d->pos(250.0f, 250.0f, 0.0f);

	return true;
}

// EOF
