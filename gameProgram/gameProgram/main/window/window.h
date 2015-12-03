//*****************************************************************************
//
// �E�B���h�E�Ǘ� [window.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_WINDOW_H_
#define MY_WINDOW_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Window
{
public:
	Window(void);
	~Window(void);

	bool Initialize(HINSTANCE hInstance, int cmdShow);
	void Finalize(void);

	static LRESULT CALLBACK WndProc(HWND hWind, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND		hWnd(void) const {return m_hWnd;}
	HINSTANCE	hInstance(void) const {return m_hInstance;}

private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
};

//=============================================================================
#endif

// EOF
