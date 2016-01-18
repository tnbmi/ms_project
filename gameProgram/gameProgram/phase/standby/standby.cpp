//*****************************************************************************
//
// �����t�F�[�Y [standby.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "standby.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\..\sound\sound.h"
#include "..\game\game.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\standby\standbyImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"
#include "..\..\phase\standby\standbyMaster\standbyMaster.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 100.0f, 800.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 150.0f, -700.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Standby::Standby(LPDIRECT3DDEVICE9 device) : Phase(device)
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

	m_standbyMaster = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Standby::~Standby(void)
{
}

//=============================================================================
// ������
//=============================================================================
bool Standby::Initialize(void)
{
	//----------------------------
	// �C���|�[�g
	//----------------------------
	if(!StandbyImport::Create(&m_import, m_device))
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
	// �T�E���h�̒�~
	//----------------------------
	Sound::StopAll();

	//----------------------------
	// �T�E���h�̍Đ�
	//----------------------------
	Sound::Play(Sound::BGM_STANDBY02);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Standby::Finalize(void)
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
	SafeFinalizeDelete( m_standbyMaster );

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
}

//=============================================================================
// �X�V
//=============================================================================
void Standby::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===�����t�F�[�Y===\n");
#endif

	//----------------------------
	// �I�u�W�F�N�g�X�V
	//----------------------------
	m_updateList->AllUpdate();

	bool isStandby = m_standbyMaster->Update();

	//----------------------------
	// ��ʑJ��
	//----------------------------
	if(isStandby)
		Manager::nextPhase((Phase*)new Game(m_device));
}

//=============================================================================
// �`��
//=============================================================================
void Standby::Draw(void)
{
	//----------------------------
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	//----------------------------
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//----------------------------
	// �I�u�W�F�N�g�`��
	//----------------------------
	m_drawListManager->AllDraw(m_camera, m_light);
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
bool Standby::InitObject(void)
{
	// �_�~�[�t���O��������
	Manager::dummyFlg(0, false);
	Manager::dummyFlg(1, false);
	Manager::dummyFlg(2, false);
	Manager::dummyFlg(3, false);

	// �����}�X�^�[
	StandbyMaster::Create( &m_standbyMaster,m_device,m_objectList,m_updateList,m_drawListManager,m_import,m_debugproc,m_padXManager,m_light );
	m_standbyMaster->keyboard(m_keyboard);

	return true;
}

// EOF
