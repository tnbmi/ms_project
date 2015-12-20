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

const int _commandData[5] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	0x0000
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
DummyPadX::DummyPadX(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	// �_�~�[�t���O
	m_dummy = true;

	// �_�~�[�̃f�[�^
	m_commandList = nullptr;
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
bool DummyPadX::Create(PadX** outPointer, int no, int patternMax, unsigned int* commandList)
{
	DummyPadX* pointer = new DummyPadX();
	if(!pointer->Initialize(no, patternMax, commandList))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool DummyPadX::Initialize(int no, int patternMax, unsigned int* commandList)
{
	//----------------------------
	// ���̓i���o�[
	//----------------------------
	m_no = no;

	//----------------------------
	// �R�}���h�ǂݍ���
	//----------------------------
	m_commandList = commandList;

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
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("***�p�b�hNo:%1d******\n", m_no);
	m_debugproc->PrintDebugProc("�I�_�~�[�I\n");
#endif

	//----------------------------
	// ���͏��X�V
	//----------------------------
	XINPUT_GAMEPAD padInput;
	padInput.wButtons		= 0x0000;
	padInput.bLeftTrigger	= 0x0000;
	padInput.bRightTrigger	= 0x0000;
	padInput.sThumbLX		= 0x0000;
	padInput.sThumbLY		= 0x0000;
	padInput.sThumbRX		= 0x0000;
	padInput.sThumbRY		= 0x0000;

	if(m_pressFlg)
	{
		padInput.wButtons = _commandData[m_commandList[m_commandPrev*10 + m_commandCnt]];
		m_pressFlg = false;
	}

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

	// �v���X
	m_state = padInput;

#ifdef _DEBUG
	// �p�b�h�\�����X�V
	m_debugproc->PrintDebugProc("---���̓R�}���h---\n");
	m_debugproc->PrintDebugProc("��:%d ��:%d ��:%d �E:%d\n",
								buttonPress(XINPUT_GAMEPAD_DPAD_UP),
								buttonPress(XINPUT_GAMEPAD_DPAD_DOWN),
								buttonPress(XINPUT_GAMEPAD_DPAD_LEFT),
								buttonPress(XINPUT_GAMEPAD_DPAD_RIGHT));
#endif
}

// [EOF]
