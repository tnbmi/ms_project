//*************	****************************************************************
//
// �L�[�{�[�h���� [keyboard.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "keyboard.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _repeatMin		= 30;	// ���s�[�g�J�n�J�E���g��
const int _repeatInterval	= 3;	// ���s�[�g�Ԋu�J�E���g��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Keyboard::Keyboard(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_input			= nullptr;
	m_inputDevice	= nullptr;

	for(int cnt = 0; cnt < KEYBOARD_MAX; ++cnt)
	{
		m_state[cnt]	 = 0x00;
		m_trigger[cnt]	 = 0x00;
		m_release[cnt]	 = 0x00;
		m_repeat[cnt]	 = 0x00;
		m_cntRepeat[cnt] = 0;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Keyboard::~Keyboard(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Keyboard::Create(Keyboard** outPointer, HINSTANCE hInstance, HWND hWnd)
{
	Keyboard* pointer = new Keyboard();
	if(!pointer->Initialize(hInstance, hWnd))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Keyboard::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	//----------------------------
	// ����
	//----------------------------
	// DirectInput�I�u�W�F�N�g����
	if(m_input == nullptr)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_input, nullptr)))
		{
			MessageBox(NULL, "�I�u�W�F�N�g�������ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
			return false;
		}
	}

	// �f�o�C�X�̐���
	if(FAILED(m_input->CreateDevice(GUID_SysKeyboard, &m_inputDevice, NULL)))
	{
		MessageBox(NULL, "�f�o�C�X�������ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return false;
	}

	// �I�u�W�F�J��
	SafeRelease(m_input);

	//----------------------------
	// �ݒ�
	//----------------------------
	// �f�[�^�t�H�[�}�b�g
	if(FAILED(m_inputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, "�f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return false;
	}

	// �������[�h
	if(FAILED(m_inputDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "�������[�h��ݒ�ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return false;
	}

	//----------------------------
	// �A�N�Z�X���擾
	//----------------------------
	m_inputDevice->Acquire();

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Keyboard::Finalize(void)
{
	//----------------------------
	// �A�N�Z�X���J��
	//----------------------------
	if(m_inputDevice != nullptr)
	{
		m_inputDevice->Unacquire();
		m_inputDevice->Release();
		m_inputDevice = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void Keyboard::Update(void)
{
	BYTE aKeyState[KEYBOARD_MAX];

	//----------------------------
	// �L�[�{�[�h��Ԃ̍X�V
	//----------------------------
	if(SUCCEEDED(m_inputDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for(int cnt = 0; cnt < KEYBOARD_MAX; ++cnt)
		{
			// ����擾
			aKeyState[cnt] &= 0x80;

			//----------------------------
			// ��ԏ��쐬
			//----------------------------
			// �g���K�[
			m_trigger[cnt] = (aKeyState[cnt] ^ m_state[cnt]) & aKeyState[cnt];

			// �����[�X
			m_release[cnt] = (aKeyState[cnt] ^ m_state[cnt]) & m_state[cnt];

			// ���s�[�g
			if(aKeyState[cnt])
			{
				// �J�E���g�A�b�v�E�L�[���Z�b�g
				++m_cntRepeat[cnt];
				m_repeat[cnt] = 0;

				if((m_cntRepeat[cnt] >= _repeatMin && (m_cntRepeat[cnt] % _repeatInterval == 0)) || m_trigger[cnt])
				{
					// ���s�[�g�J�n
					m_repeat[cnt] = aKeyState[cnt];
				}
			}
			else
			{
				m_repeat[cnt] = aKeyState[cnt];
				m_cntRepeat[cnt] = 0;
			}

			// �v���X
			m_state[cnt] = aKeyState[cnt];
		}
	}
	else
	{
		// �A�N�Z�X����蒼��
		m_inputDevice->Acquire();
	}
}

// [EOF]
