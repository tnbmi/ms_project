//*****************************************************************************
//
// ウィンドウ管理 [window.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "window.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// ウィンドウ設定
//----------------------------
const LPCSTR _windowClass	= "WindowClass";	// ウインドウのクラス名
const LPCSTR _windowCaption	= "親玄舞闘会";		// ウインドウのキャプション名

//=============================================================================
// コンストラクタ
//=============================================================================
Window::Window(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_hInstance	= nullptr;
	m_hWnd		= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Window::~Window(void)
{
}

//=============================================================================
// 初期化
//=============================================================================
bool Window::Initialize(HINSTANCE hInstance, int cmdShow)
{
	//----------------------------
	// インスタンスハンドル
	//----------------------------
	m_hInstance = hInstance;

	//----------------------------
	// ウィンドウクラス設定
	//----------------------------
	WNDCLASSEX wndClassEX =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,						// ウインドウのレイアウト
		Window::WndProc,				// プロシージャ関数
		0,
		0,
		m_hInstance,					// インスタンスハンドル
		nullptr,						// アイコン指定
		LoadCursor(nullptr, IDC_ARROW),	// マウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 0),		// 背景
		nullptr,						// メニューの表示
		_windowClass,					// ウインドウクラスの名前
		nullptr							// 小さいアイコン指定
	};

	//----------------------------
	// ウィンドウクラスの登録
	//----------------------------
	RegisterClassEx(&wndClassEX);

	//----------------------------
	// ウィンドウ生成
	//----------------------------
	int width  = SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int height = SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	m_hWnd = CreateWindowEx(0,
							_windowClass,			// クラスの名前
							_windowCaption,			// キャプション名
							WS_OVERLAPPEDWINDOW &~WS_THICKFRAME,	// スタイル
							CW_USEDEFAULT,			// X座標
							CW_USEDEFAULT,			// Y座標
							width,					// 横幅
							height,					// 縦幅
							nullptr,				// 親ウィンドウハンドル
							nullptr,				// アプリインスタンスハンドル
							m_hInstance,			// インスタンスハンドル
							nullptr);				// ウィンドウ作成データ

	// ウィンドウハンドルのチェック
	if(m_hWnd == nullptr)
	{
		MessageBox(NULL, "Window", "ウィンドウハンドル生成時にエラーが発生しました。", MB_OK);
		return false;
	}

	//----------------------------
	// ウインドウの表示(
	//----------------------------
	ShowWindow(m_hWnd, cmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Window::Finalize(void)
{
	//----------------------------
	// ウィンドウ登録の解除
	//----------------------------
	UnregisterClass(_windowClass, m_hInstance);
}

//=============================================================================
// ウィンドウプロシージャ
//=============================================================================
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//----------------------------
	// メッセージ処理
	//----------------------------
	switch(msg)
	{
	// ウインドウ破棄
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	// ウィンドウ閉め
	case WM_CLOSE:
		DestroyWindow(hWnd);
			break;

	// キーボード処理
	case WM_KEYDOWN:
		switch(wParam)
		{
		// ESCが押されたとき
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
