//*****************************************************************************
//
// XInput�Q�[���p�b�h���� [padX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "padX.h"
#include "..\..\debugproc\debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _repeatMin		= 30;	// ���s�[�g�J�n�J�E���g��
const int _repeatInterval	= 3;	// ���s�[�g�Ԋu�J�E���g��

// �p�b�h�t���O�m�F�p
const WORD _padButton[BUTTON_NUM] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
PadX::PadX(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_debugproc = nullptr;

	// �ڑ����
	m_connected = false;

	// ���͏��
	m_state.wButtons		= 0;
	m_state.bLeftTrigger	= 0;
	m_state.bRightTrigger	= 0;
	m_state.sThumbLX		= 0;
	m_state.sThumbLY		= 0;
	m_state.sThumbRX		= 0;
	m_state.sThumbRY		= 0;

	m_trigger.wButtons		= 0;
	m_trigger.bLeftTrigger	= 0;
	m_trigger.bRightTrigger	= 0;
	m_trigger.sThumbLX		= 0;
	m_trigger.sThumbLY		= 0;
	m_trigger.sThumbRX		= 0;
	m_trigger.sThumbRY		= 0;

	m_release.wButtons		= 0;
	m_release.bLeftTrigger	= 0;
	m_release.bRightTrigger	= 0;
	m_release.sThumbLX		= 0;
	m_release.sThumbLY		= 0;
	m_release.sThumbRX		= 0;
	m_release.sThumbRY		= 0;

	m_repeat.wButtons		= 0;
	m_repeat.bLeftTrigger	= 0;
	m_repeat.bRightTrigger	= 0;
	m_repeat.sThumbLX		= 0;
	m_repeat.sThumbLY		= 0;
	m_repeat.sThumbRX		= 0;
	m_repeat.sThumbRY		= 0;

	// ���s�[�g�J�E���g
	for(int cnt = 0; cnt < BUTTON_NUM; ++cnt)
		m_cntRepeatBtn[cnt] = 0;
	for(int cnt = 0; cnt < TRIGGER_NUM; ++cnt)
		m_cntRepeatTrigger[cnt] = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
PadX::~PadX(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool PadX::Create(PadX** outPointer, int no)
{
	PadX* pointer = new PadX();
	if(!pointer->Initialize(no))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool PadX::Initialize(int no)
{
	//----------------------------
	// ���g�̃i���o�[�m��
	//----------------------------
	m_no = no;

	//----------------------------
	// �p�b�h�ڑ����擾
	//----------------------------
	XINPUT_STATE pad;

	if(XInputGetState(m_no, &pad) != ERROR_SUCCESS)
	{
		// �q�����Ă��Ȃ�
		m_connected = false;
	}
	else
	{
		// �q�����Ă���
		m_connected = true;
	}

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void PadX::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void PadX::Update(void)
{
	//----------------------------
	// �p�b�h���X�V
	//----------------------------
	XINPUT_STATE	pad;

#ifdef _DEBUG
	m_debugproc->PrintDebugProc("***�p�b�hNo:%1d******\n", m_no);
#endif

	if(XInputGetState(m_no, &pad) != ERROR_SUCCESS)
	{
		// �q�����Ă��Ȃ�
		m_connected = false;

#ifdef _DEBUG
		m_debugproc->PrintDebugProc("���ڑ�\n", m_no);
#endif
	}
	else
	{
		// �q�����Ă���
		m_connected = true;

		//----------------------------
		// ���͏��X�V
		//----------------------------
		XINPUT_GAMEPAD padInput = pad.Gamepad;

		// �g���K�[
		m_trigger.wButtons		= (padInput.wButtons	  ^ m_state.wButtons)		& padInput.wButtons;
		m_trigger.bLeftTrigger	= (padInput.bLeftTrigger  ^ m_state.bLeftTrigger)	& padInput.bLeftTrigger;
		m_trigger.bRightTrigger	= (padInput.bRightTrigger ^ m_state.bRightTrigger)	& padInput.bRightTrigger;
		m_trigger.sThumbLX		= (padInput.sThumbLX	  ^ m_state.sThumbLX)		& padInput.sThumbLX;
		m_trigger.sThumbLY		= (padInput.sThumbLY	  ^ m_state.sThumbLY)		& padInput.sThumbLY;
		m_trigger.sThumbRX		= (padInput.sThumbRX	  ^ m_state.sThumbRX)		& padInput.sThumbRX;
		m_trigger.sThumbRY		= (padInput.sThumbRY	  ^ m_state.sThumbRY)		& padInput.sThumbRY;

		// �����[�X
		m_release.wButtons		= (padInput.wButtons	  ^ m_state.wButtons)		& m_state.wButtons;
		m_release.bLeftTrigger	= (padInput.bLeftTrigger  ^ m_state.bLeftTrigger)	& m_state.bLeftTrigger;
		m_release.bRightTrigger	= (padInput.bRightTrigger ^ m_state.bRightTrigger)	& m_state.bRightTrigger;
		m_release.sThumbLX		= (padInput.sThumbLX	  ^ m_state.sThumbLX)		& m_state.sThumbLX;
		m_release.sThumbLY		= (padInput.sThumbLY	  ^ m_state.sThumbLY)		& m_state.sThumbLY;
		m_release.sThumbRX		= (padInput.sThumbRX	  ^ m_state.sThumbRX)		& m_state.sThumbRX;
		m_release.sThumbRY		= (padInput.sThumbRY	  ^ m_state.sThumbRY)		& m_state.sThumbRY;

/* ---�l����---
		// ���s�[�g
		for(int cnt = 0; cnt < BUTTON_NUM; ++cnt)
		{
			if(padInput.wButtons & PAD_BUTTON[cnt])
			{
				// �J�E���g�A�b�v�E�L�[���Z�b�g
				++m_cntRepeatBtn[cnt];
				m_repeat.wButtons = PAD_BUTTON[cnt];
			}
		}
		m_repeat.wButtons		= (padInput.wButtons	  ^ m_state.wButtons)		& m_state.wButtons;
		m_repeat.bLeftTrigger	= (padInput.bLeftTrigger  ^ m_state.bLeftTrigger)	& m_state.bLeftTrigger;
		m_repeat.bRightTrigger	= (padInput.bRightTrigger ^ m_state.bRightTrigger)	& m_state.bRightTrigger;
		m_repeat.sThumbLX		= (padInput.sThumbLX	  ^ m_state.sThumbLX)		& m_state.sThumbLX;
		m_repeat.sThumbLY		= (padInput.sThumbLY	  ^ m_state.sThumbLY)		& m_state.sThumbLY;
		m_repeat.sThumbRX		= (padInput.sThumbRX	  ^ m_state.sThumbRX)		& m_state.sThumbRX;
		m_repeat.sThumbRY		= (padInput.sThumbRY	  ^ m_state.sThumbRY)		& m_state.sThumbRY;
*/
		// �v���X
		m_state = padInput;

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

/*		m_debugproc->PrintDebugProc("---Trigger---\n");
		m_debugproc->PrintDebugProc("��:%d ��:%d ��:%d �E:%d | A:%d B:%d X:%d Y:%d\n",
									buttonTrigger(XINPUT_GAMEPAD_DPAD_UP),
									buttonTrigger(XINPUT_GAMEPAD_DPAD_DOWN),
									buttonTrigger(XINPUT_GAMEPAD_DPAD_LEFT),
									buttonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT),
									buttonTrigger(XINPUT_GAMEPAD_A),
									buttonTrigger(XINPUT_GAMEPAD_B),
									buttonTrigger(XINPUT_GAMEPAD_X),
									buttonTrigger(XINPUT_GAMEPAD_Y));
		m_debugproc->PrintDebugProc("LB:%d RB:%d LT:%d RT:%d | LThumb:%d RThumb:%d\n",
									buttonTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER),
									buttonTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER),
									triggerLTrigger(),
									triggerRTrigger(),
									buttonTrigger(XINPUT_GAMEPAD_LEFT_THUMB),
									buttonTrigger(XINPUT_GAMEPAD_RIGHT_THUMB));
		m_debugproc->PrintDebugProc("start:%d back:%d\n",
									buttonTrigger(XINPUT_GAMEPAD_START),
									buttonTrigger(XINPUT_GAMEPAD_BACK));

		m_debugproc->PrintDebugProc("---Release---\n");
		m_debugproc->PrintDebugProc("��:%d ��:%d ��:%d �E:%d | A:%d B:%d X:%d Y:%d\n",
									buttonRelease(XINPUT_GAMEPAD_DPAD_UP),
									buttonRelease(XINPUT_GAMEPAD_DPAD_DOWN),
									buttonRelease(XINPUT_GAMEPAD_DPAD_LEFT),
									buttonRelease(XINPUT_GAMEPAD_DPAD_RIGHT),
									buttonRelease(XINPUT_GAMEPAD_A),
									buttonRelease(XINPUT_GAMEPAD_B),
									buttonRelease(XINPUT_GAMEPAD_X),
									buttonRelease(XINPUT_GAMEPAD_Y));
		m_debugproc->PrintDebugProc("LB:%d RB:%d LT:%d RT:%d | LThumb:%d RThumb:%d\n",
									buttonRelease(XINPUT_GAMEPAD_LEFT_SHOULDER),
									buttonRelease(XINPUT_GAMEPAD_RIGHT_SHOULDER),
									triggerLRelease(),
									triggerRRelease(),
									buttonRelease(XINPUT_GAMEPAD_LEFT_THUMB),
									buttonRelease(XINPUT_GAMEPAD_RIGHT_THUMB));
		m_debugproc->PrintDebugProc("start:%d back:%d\n",
									buttonRelease(XINPUT_GAMEPAD_START),
									buttonRelease(XINPUT_GAMEPAD_BACK));*/
#endif
	}
}

// [EOF]
