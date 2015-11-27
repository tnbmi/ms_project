//*****************************************************************************
//
// ���C���}�l�[�W���[ [manager.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "manager.h"
#include "..\common\safe.h"

#include "..\renderer\renderer.h"
#include "..\debugproc\debugproc.h"

#include "..\input\keyboard\keyboard.h"
#include "..\input\padX\padXManager.h"

#include "..\phase\title\title.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Phase* Manager::m_phase		= nullptr;
Phase* Manager::m_nextPhase	= nullptr;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Manager::Manager(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_countFPS	= 0;

	m_phase		= nullptr;
	m_nextPhase	= nullptr;

	m_renderer	= nullptr;
	m_debugproc	= nullptr;

	m_keyboard		= nullptr;
	m_padXManager	= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Manager::~Manager(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Manager::Create(Manager** outPointer, HINSTANCE hInstance, HWND hWnd, bool windowFlg)
{
	Manager* pointer = new Manager();
	if(!pointer->Initialize(hInstance, hWnd, windowFlg))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Manager::Initialize(HINSTANCE hInstance, HWND hWnd, bool windowFlg)
{
	//----------------------------
	// �����_���[
	//----------------------------
	if(!Renderer::Create(&m_renderer, hWnd, windowFlg))
		return false;
	LPDIRECT3DDEVICE9 device = m_renderer->device();

	//----------------------------
	// ����
	//----------------------------
	if(!Keyboard::Create(&m_keyboard, hInstance, hWnd))
		return false;
	if(!PadXManager::Create(&m_padXManager))
		return false;

#ifdef _DEBUG
	//----------------------------
	// �f�o�b�O�R�����g
	//----------------------------
	if(!Debugproc::Create(&m_debugproc, device))
		return false;
	m_renderer->debugproc(m_debugproc);
	m_padXManager->debugproc(m_debugproc);
#endif

	//----------------------------
	// �t�F�[�Y
	//----------------------------
	// ����
	m_phase = (Phase*)new Title(device);

	// ���͐ݒ�
	m_phase->padXManager(m_padXManager);
	m_phase->keyboard(m_keyboard);

#ifdef _DEBUG
	// �f�o�b�O�ݒ�
	m_phase->debugproc(m_debugproc);
#endif

	// ������
	if(!m_phase->Initialize())
		return false;
	m_nextPhase = m_phase;
	m_renderer->phase(m_phase);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Manager::Finalize(void)
{
	//----------------------------
	// �t�F�[�Y
	//----------------------------
	SafeFinalizeDelete(m_phase);

#ifdef _DEBUG
	//----------------------------
	// �f�o�b�O�R�����g
	//----------------------------
	SafeFinalizeDelete(m_debugproc);
#endif

	//----------------------------
	// ����
	//----------------------------
	SafeFinalizeDelete(m_keyboard);
	SafeFinalizeDelete(m_padXManager);

	//----------------------------
	// �����_���[
	//----------------------------
	SafeFinalizeDelete(m_renderer);
}

//=============================================================================
// �X�V
//=============================================================================
bool Manager::Update(void)
{
#ifdef _DEBUG
	//----------------------------
	// �f�o�b�O�\��
	//----------------------------
	m_debugproc->Update();
	m_debugproc->PrintDebugProc("FPS:%d\n", m_countFPS);
#endif

	//----------------------------
	// ����
	//----------------------------
	m_keyboard->Update();
	m_padXManager->Update();

	//----------------------------
	// �����_���[�A�T�E���h
	//----------------------------
	// �����_���[
	m_renderer->Update();

	//----------------------------
	// �t�F�[�Y�ؑ�
	//----------------------------
	if(m_nextPhase != m_phase)
	{
		// ���݃t�F�[�Y��j��
		SafeFinalizeDelete(m_phase);

		// ���͐ݒ�
		m_nextPhase->padXManager(m_padXManager);
		m_nextPhase->keyboard(m_keyboard);

#ifdef _DEBUG
		// �f�o�b�O�ݒ�
		m_nextPhase->debugproc(m_debugproc);
#endif

		// ���̃t�F�[�Y���������E�ݒ�
		if(!m_nextPhase->Initialize())
			return false;
		m_phase = m_nextPhase;
		m_renderer->phase(m_phase);
	}

	return true;
}

//=============================================================================
// �`��
//=============================================================================
void Manager::Draw(void)
{
	//----------------------------
	// �t�F�[�Y
	//----------------------------
	m_phase->Draw();

	//----------------------------
	// �����_���[
	//----------------------------
	m_renderer->Draw();
}

//=============================================================================
// FPS����
//=============================================================================
void Manager::CalculateFPS(DWORD frameCnt, DWORD curTime, DWORD FPSLastTime)
{
	m_countFPS = frameCnt * 1000 / (curTime - FPSLastTime);
}

// [EOF]
