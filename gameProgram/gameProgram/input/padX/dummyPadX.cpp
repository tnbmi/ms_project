//*****************************************************************************
//
// �ւ���XInput�Q�[���p�b�h���� [dummyPadX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dummyPadX.h"
#include "..\..\debugproc\debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _repeatMin		= 30;	// ���s�[�g�J�n�J�E���g��
const int _repeatInterval	= 3;	// ���s�[�g�Ԋu�J�E���g��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
DummyPadX::DummyPadX(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_debugproc = nullptr;

	// ���͏��
	m_state.wButtons		= 0x0000;
	m_state.bLeftTrigger	= 0x0000;
	m_state.bRightTrigger	= 0x0000;
	m_state.sThumbLX		= 0x0000;
	m_state.sThumbLY		= 0x0000;
	m_state.sThumbRX		= 0x0000;
	m_state.sThumbRY		= 0x0000;

	m_trigger.wButtons		= 0x0000;
	m_trigger.bLeftTrigger	= 0x0000;
	m_trigger.bRightTrigger	= 0x0000;
	m_trigger.sThumbLX		= 0x0000;
	m_trigger.sThumbLY		= 0x0000;
	m_trigger.sThumbRX		= 0x0000;
	m_trigger.sThumbRY		= 0x0000;

	m_release.wButtons		= 0x0000;
	m_release.bLeftTrigger	= 0x0000;
	m_release.bRightTrigger	= 0x0000;
	m_release.sThumbLX		= 0x0000;
	m_release.sThumbLY		= 0x0000;
	m_release.sThumbRX		= 0x0000;
	m_release.sThumbRY		= 0x0000;

	m_repeat.wButtons		= 0x0000;
	m_repeat.bLeftTrigger	= 0x0000;
	m_repeat.bRightTrigger	= 0x0000;
	m_repeat.sThumbLX		= 0x0000;
	m_repeat.sThumbLY		= 0x0000;
	m_repeat.sThumbRX		= 0x0000;
	m_repeat.sThumbRY		= 0x0000;

	// ���s�[�g�J�E���g
	for(int cnt = 0; cnt < BUTTON_NUM; ++cnt)
		m_cntRepeatBtn[cnt] = 0;
	for(int cnt = 0; cnt < TRIGGER_NUM; ++cnt)
		m_cntRepeatTrigger[cnt] = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
DummyPadX::~DummyPadX(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool DummyPadX::Create(DummyPadX** outPointer, int no)
{
	DummyPadX* pointer = new DummyPadX();
	if(!pointer->Initialize(no))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool DummyPadX::Initialize(int no)
{
	//----------------------------
	// �R�}���h�ǂݍ���
	//----------------------------

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void DummyPadX::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void DummyPadX::Update(void)
{
	//----------------------------
	// �p�b�h���X�V
	//----------------------------
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("***�p�b�hNo:%1d******\n", m_no);
	m_debugproc->PrintDebugProc("�I�_�~�[�I\n", m_no);
#endif

	//----------------------------
	// ���͏��X�V
	//----------------------------
	// �g���K�[
	m_trigger.wButtons		= 0x0000;
	m_trigger.bLeftTrigger	= 0x0000;
	m_trigger.bRightTrigger	= 0x0000;
	m_trigger.sThumbLX		= 0x0000;
	m_trigger.sThumbLY		= 0x0000;
	m_trigger.sThumbRX		= 0x0000;
	m_trigger.sThumbRY		= 0x0000;

	// �����[�X
	m_release.wButtons		= 0x0000;
	m_release.bLeftTrigger	= 0x0000;
	m_release.bRightTrigger	= 0x0000;
	m_release.sThumbLX		= 0x0000;
	m_release.sThumbLY		= 0x0000;
	m_release.sThumbRX		= 0x0000;
	m_release.sThumbRY		= 0x0000;

	// �v���X
	m_state.wButtons		= 0x0000;
	m_state.bLeftTrigger	= 0x0000;
	m_state.bRightTrigger	= 0x0000;
	m_state.sThumbLX		= 0x0000;
	m_state.sThumbLY		= 0x0000;
	m_state.sThumbRX		= 0x0000;
	m_state.sThumbRY		= 0x0000;

#ifdef _DEBUG
	// �p�b�h�\�����X�V
	m_debugproc->PrintDebugProc("StickL X:%d Y:%d StickR X:%d Y:%d\n",
								thumbLX(),
								thumbLY(),
								thumbRX(),
								thumbRY());

	m_debugproc->PrintDebugProc("---Press---\n");
	m_debugproc->PrintDebugProc("��:%d ��:%d ��:%d �E:%d | A:%d B:%d X:%d Y:%d\n",
								buttonPress(XINPUT_GAMEPAD_DPAD_UP),
								buttonPress(XINPUT_GAMEPAD_DPAD_DOWN),
								buttonPress(XINPUT_GAMEPAD_DPAD_LEFT),
								buttonPress(XINPUT_GAMEPAD_DPAD_RIGHT),
								buttonPress(XINPUT_GAMEPAD_A),
								buttonPress(XINPUT_GAMEPAD_B),
								buttonPress(XINPUT_GAMEPAD_X),
								buttonPress(XINPUT_GAMEPAD_Y));
	m_debugproc->PrintDebugProc("LB:%d RB:%d LT:%d RT:%d | LThumb:%d RThumb:%d\n",
								buttonPress(XINPUT_GAMEPAD_LEFT_SHOULDER),
								buttonPress(XINPUT_GAMEPAD_RIGHT_SHOULDER),
								triggerLPress(),
								triggerRPress(),
								buttonPress(XINPUT_GAMEPAD_LEFT_THUMB),
								buttonPress(XINPUT_GAMEPAD_RIGHT_THUMB));
	m_debugproc->PrintDebugProc("start:%d back:%d\n",
								buttonPress(XINPUT_GAMEPAD_START),
								buttonPress(XINPUT_GAMEPAD_BACK));
#endif
}

// [EOF]
