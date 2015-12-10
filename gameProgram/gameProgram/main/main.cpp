//*****************************************************************************
//
// ���C�� [main.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "..\common\safe.h"

#include "window\window.h"
#include "..\manager\manager.h"

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//----------------------------
	// �������[���[�N�`�F�b�N
	//----------------------------
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//----------------------------
	// ���g�p�錾
	//----------------------------
	UNREFERENCED_PARAMETER(prevInstance);	// �����Ă��ǂ����ǁA�x�����o��
	UNREFERENCED_PARAMETER(cmdLine);		// �����Ă��ǂ����ǁA�x�����o��

	//----------------------------
	// �E�B���h�E����
	//----------------------------
	Window* window = new Window;
	if(!window->Initialize(hInstance, cmdShow))
		return -1;

	//----------------------------
	// �t���X�N���[���ݒ�
	//----------------------------
	bool windowFlg = true;
	if(MessageBox(NULL,"�t���X�N���[���ŋN�����܂���","�t���X�N���[���ݒ�",MB_YESNO)==IDYES)
		//�t���X�N���[���ŏ���������(�E�B���h�E���쐬���Ă���s��)
		windowFlg = FALSE;
	else
		//�ʏ�� ����������(�E�B���h�E���쐬���Ă���s��)
		windowFlg = TRUE;

	//----------------------------
	// �}�l�[�W���[����
	//----------------------------
	Manager* manager = nullptr;
	if(!Manager::Create(&manager, hInstance, window->hWnd(), windowFlg))
		return -1;

	//----------------------------
	// �t���[���J�E���g
	//----------------------------
	DWORD execLastTime;	// ��������
	DWORD FPSLastTime;	// FPS���莞��
	DWORD curTime;		// ���ݎ���
	DWORD frameCnt;		// �t���[���J�E���g

	timeBeginPeriod(1);				// ����\��ݒ�
	execLastTime = timeGetTime();
	FPSLastTime	 = execLastTime;	// �V�X�e���������~���b�P�ʂŎ擾
	curTime		 =	 0;				// ���ݎ����ƃJ�E���g�̏�����
	frameCnt	 = curTime;

	//----------------------------
	// ���b�Z�[�W���[�v
	//----------------------------
	MSG msg;

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{
			if(msg.message == WM_QUIT)
			{// ���[�v�I��
				break;
			}
			else
			{// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			curTime = timeGetTime();			// �V�X�e���������擾
			if((curTime - FPSLastTime) >= 500)
			{// FPS�𑪒�
				manager->CalculateFPS(frameCnt, curTime, FPSLastTime);
				FPSLastTime = curTime;
				frameCnt = 0;
			}

			if((curTime - execLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				execLastTime = curTime;	// ��������������ۑ�
				++frameCnt;

				manager->Update();
				manager->Draw();
			}
		}
	}

	//----------------------------
	// �I������
	//----------------------------
	// �t���[���J�E���g
	timeEndPeriod(1);

	// �}�l�[�W���[
	SafeFinalizeDelete(manager);

	// �E�B���h�E
	SafeFinalizeDelete(window);

	return (int)msg.wParam;
}

// EOF
