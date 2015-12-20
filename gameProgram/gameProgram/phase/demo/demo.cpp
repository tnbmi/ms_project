//*****************************************************************************
//
// �f���t�F�[�Y [demo.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "demo.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"
#include "..\..\manager\manager.h"
#include "..\..\sound\sound.h"
#include "..\title\title.h"

#include "..\..\input\keyboard\keyboard.h"
#include "..\..\input\padX\padXManager.h"
#include "..\..\input\padX\padX.h"

#include "..\..\import\demo\demoImport.h"

#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\view\light\light.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\commandmanager\commandmanager.h"
#include "..\..\timemanager\timeManager.h"

#include "..\..\objectBase\polygon3D\polygon3D.h"
#include "..\..\objectBase\meshDome\meshDome.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"

#include "..\..\object\player\player.h"

#include "..\..\phase\demo\demoMaster\demoMaster.h"
#include "..\..\import\fbx\fbxTexImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 1050.0f, 10000.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 350.0f, -2250.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Demo::Demo(LPDIRECT3DDEVICE9 device) : Phase(device)
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
	m_fbxTexImport = nullptr;

	m_demoMaster = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Demo::~Demo(void)
{
}

//=============================================================================
// ������
//=============================================================================
bool Demo::Initialize(void)
{
	//----------------------------
	// �C���|�[�g
	//----------------------------
	if(!DemoImport::Create(&m_import, m_device))
		return false;
	//if(!FbxTexImport::Create(&m_fbxTexImport,m_device ))
	//	return false;
	m_fbxTexImport = Manager::fbxTexImport();

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
	// �T�E���h�̒�~
	//----------------------------
	Sound::StopAll();

	//----------------------------
	// �T�E���h�̍Đ�
	//----------------------------
	Sound::Play(Sound::BGM_GAME01);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Demo::Finalize(void)
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

	//�f���}�X�^�[
	SafeFinalizeDelete( m_demoMaster );

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
	//SafeFinalizeDelete(m_fbxTexImport);
}

//=============================================================================
// �X�V
//=============================================================================
void Demo::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("===�f���t�F�[�Y===\n");
#endif

	//----------------------------
	// �J�����ݒ�
	//----------------------------
	m_camera->SetCamera();

	//----------------------------
	// �I�u�W�F�N�g�X�V
	//----------------------------
	m_updateList->AllUpdate();

	//----------------------------
	//DemoMaster�X�V
	//----------------------------
	if(m_demoMaster->Update() || m_padXManager->InputChk() || m_keyboard->trigger(DIK_RETURN))
		Manager::nextPhase((Phase*)new Title(m_device));
}

//=============================================================================
// �`��
//=============================================================================
void Demo::Draw(void)
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
bool Demo::InitObject(void)
{
	//----------------------------
	// �X�R�A������
	//----------------------------
	Manager::score(0, 0);

	//----------------------------
	// �󃁃b�V���h�[��
	//----------------------------
	MeshDome* dome;
	if(!MeshDome::Create(&dome, m_device, m_objectList,
		D3DXVECTOR2(8, 3), 2500.0f, 2000.0f, m_import->texture(DemoImport::SKY)))
		return false;
	m_updateList->Link(dome);
	m_drawListManager->Link(dome, 0, Shader::PAT_NONE_LIGHT);
	dome->pos_y(-150.0f);
	dome->rot_y(PAI * 0.5f);

	//----------------------------
	// �w�i3D�|���S��
	//----------------------------
	Polygon3D* bg3D;
	if(!Polygon3D::Create(&bg3D, m_device, m_objectList, m_import->texture(DemoImport::BG)))
		return false;
	m_updateList->Link(bg3D);
	m_drawListManager->Link(bg3D, 1, Shader::PAT_NONE_LIGHT);
	D3DXVECTOR3 size = D3DXVECTOR3(1800.0f*0.6f, 720.0f*0.6f, 0.0f);
	bg3D->scl(size);
	bg3D->pos(0.0f, size.y*0.5f, 512.0f*2.5f);
	bg3D->texcoord(0, 0.0f, 0.01f);
	bg3D->texcoord(1, 1.0f, 0.01f);

	//----------------------------
	// �n��3D�|���S��
	//----------------------------
	Polygon3D* ground;
	if(!Polygon3D::Create(&ground, m_device, m_objectList, m_import->texture(DemoImport::GROUND)))
		return false;
	m_updateList->Link(ground);
	m_drawListManager->Link(ground, 2, Shader::PAT_NONE_LIGHT);
	ground->scl(512.0f*5, 512.0f*5, 0.0f);
	ground->rot_x(PAI * 0.5f);

	//----------------------------
	//�f���}�X�^�[����
	//----------------------------
	DemoMaster::Create( &m_demoMaster,m_device,m_objectList,m_updateList,m_drawListManager,m_import,m_fbxTexImport,m_debugproc,m_padXManager);

	return true;
}

// EOF
