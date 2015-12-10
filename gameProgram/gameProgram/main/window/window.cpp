//*****************************************************************************
//
// �E�B���h�E�Ǘ� [window.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "window.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �E�B���h�E�ݒ�
//----------------------------
const LPCSTR _windowClass	= "WindowClass";	// �E�C���h�E�̃N���X��
const LPCSTR _windowCaption	= "�e��������";		// �E�C���h�E�̃L���v�V������

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Window::Window(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_hInstance	= nullptr;
	m_hWnd		= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Window::~Window(void)
{
}

//=============================================================================
// ������
//=============================================================================
bool Window::Initialize(HINSTANCE hInstance, int cmdShow)
{
	//----------------------------
	// �C���X�^���X�n���h��
	//----------------------------
	m_hInstance = hInstance;

	//----------------------------
	// �E�B���h�E�N���X�ݒ�
	//----------------------------
	WNDCLASSEX wndClassEX =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,						// �E�C���h�E�̃��C�A�E�g
		Window::WndProc,				// �v���V�[�W���֐�
		0,
		0,
		m_hInstance,					// �C���X�^���X�n���h��
		nullptr,						// �A�C�R���w��
		LoadCursor(nullptr, IDC_ARROW),	// �}�E�X�J�[�\���w��
		(HBRUSH)(COLOR_WINDOW + 0),		// �w�i
		nullptr,						// ���j���[�̕\��
		_windowClass,					// �E�C���h�E�N���X�̖��O
		nullptr							// �������A�C�R���w��
	};

	//----------------------------
	// �E�B���h�E�N���X�̓o�^
	//----------------------------
	RegisterClassEx(&wndClassEX);

	//----------------------------
	// �E�B���h�E����
	//----------------------------
	int width  = SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int height = SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	m_hWnd = CreateWindowEx(0,
							_windowClass,			// �N���X�̖��O
							_windowCaption,			// �L���v�V������
							WS_OVERLAPPEDWINDOW &~WS_THICKFRAME,	// �X�^�C��
							CW_USEDEFAULT,			// X���W
							CW_USEDEFAULT,			// Y���W
							width,					// ����
							height,					// �c��
							nullptr,				// �e�E�B���h�E�n���h��
							nullptr,				// �A�v���C���X�^���X�n���h��
							m_hInstance,			// �C���X�^���X�n���h��
							nullptr);				// �E�B���h�E�쐬�f�[�^

	// �E�B���h�E�n���h���̃`�F�b�N
	if(m_hWnd == nullptr)
	{
		MessageBox(NULL, "Window", "�E�B���h�E�n���h���������ɃG���[���������܂����B", MB_OK);
		return false;
	}

	//----------------------------
	// �E�C���h�E�̕\��(
	//----------------------------
	ShowWindow(m_hWnd, cmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Window::Finalize(void)
{
	//----------------------------
	// �E�B���h�E�o�^�̉���
	//----------------------------
	UnregisterClass(_windowClass, m_hInstance);
}

//=============================================================================
// �E�B���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//----------------------------
	// ���b�Z�[�W����
	//----------------------------
	switch(msg)
	{
	// �E�C���h�E����
	case WM_CREATE:
		break;

	// �E�C���h�E�j��
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// �E�B���h�E��
	case WM_CLOSE:
		DestroyWindow(hWnd);
			break;

	// �L�[�{�[�h����
	case WM_KEYDOWN:
		switch(wParam)
		{
		// ESC�������ꂽ�Ƃ�
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// EOF
