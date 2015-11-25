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

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\game\gameImport.h"
#include "..\..\view\camera\camera.h"

#include "..\..\commandmanager\commandmanager.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 0.0f, -50.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Game::Game(LPDIRECT3DDEVICE9 device) : Phase(device)
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
Game::~Game(void)
{
}

//=============================================================================
// ������
//=============================================================================
bool Game::Initialize(void)
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
	if(!GameImport::Create(&m_import, m_device))
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
void Game::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===�Q�[���t�F�[�Y===\n");
#endif

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
	// ��ʑJ��
	//----------------------------
	if(pad->buttonTrigger(XINPUT_GAMEPAD_A))
	{
		//Manager::nextPhase((Phase*)new Result(m_device));
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
	// �J�����Z�b�g
	//----------------------------
	m_camera->SetCamera();

	//----------------------------
	// 2D�`��
	//----------------------------
	m_drawListManager->AllDraw(m_camera->viewProjection());

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
	// �R�}���h�}�l�[�W������
	Commandmanager::Create(&m_command_manager, m_padXManager, m_debugproc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);

	return true;
}

// EOF
