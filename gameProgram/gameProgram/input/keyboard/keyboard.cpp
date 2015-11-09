//*************	****************************************************************
//
// キーボード入力 [keyboard.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "keyboard.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _repeatMin		= 30;	// リピート開始カウント数
const int _repeatInterval	= 3;	// リピート間隔カウント数

//=============================================================================
// コンストラクタ
//=============================================================================
Keyboard::Keyboard(void)
{
	//----------------------------
	// メンバー初期化
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
// デストラクタ
//=============================================================================
Keyboard::~Keyboard(void)
{
}

//=============================================================================
// 生成
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
// 初期化
//=============================================================================
bool Keyboard::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	//----------------------------
	// 生成
	//----------------------------
	// DirectInputオブジェクト生成
	if(m_input == nullptr)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_input, nullptr)))
		{
			MessageBox(NULL, "オブジェクトが生成できませんでした", "エラー", (MB_OK | MB_ICONERROR));
			return false;
		}
	}

	// デバイスの生成
	if(FAILED(m_input->CreateDevice(GUID_SysKeyboard, &m_inputDevice, NULL)))
	{
		MessageBox(NULL, "デバイスが生成できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return false;
	}

	// オブジェ開放
	SafeRelease(m_input);

	//----------------------------
	// 設定
	//----------------------------
	// データフォーマット
	if(FAILED(m_inputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, "データフォーマットを設定できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return false;
	}

	// 協調モード
	if(FAILED(m_inputDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "協調モードを設定できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return false;
	}

	//----------------------------
	// アクセス権取得
	//----------------------------
	m_inputDevice->Acquire();

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Keyboard::Finalize(void)
{
	//----------------------------
	// アクセス権開放
	//----------------------------
	if(m_inputDevice != nullptr)
	{
		m_inputDevice->Unacquire();
		m_inputDevice->Release();
		m_inputDevice = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void Keyboard::Update(void)
{
	BYTE aKeyState[KEYBOARD_MAX];

	//----------------------------
	// キーボード状態の更新
	//----------------------------
	if(SUCCEEDED(m_inputDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for(int cnt = 0; cnt < KEYBOARD_MAX; ++cnt)
		{
			// 現状取得
			aKeyState[cnt] &= 0x80;

			//----------------------------
			// 状態情報作成
			//----------------------------
			// トリガー
			m_trigger[cnt] = (aKeyState[cnt] ^ m_state[cnt]) & aKeyState[cnt];

			// リリース
			m_release[cnt] = (aKeyState[cnt] ^ m_state[cnt]) & m_state[cnt];

			// リピート
			if(aKeyState[cnt])
			{
				// カウントアップ・キーリセット
				++m_cntRepeat[cnt];
				m_repeat[cnt] = 0;

				if((m_cntRepeat[cnt] >= _repeatMin && (m_cntRepeat[cnt] % _repeatInterval == 0)) || m_trigger[cnt])
				{
					// リピート開始
					m_repeat[cnt] = aKeyState[cnt];
				}
			}
			else
			{
				m_repeat[cnt] = aKeyState[cnt];
				m_cntRepeat[cnt] = 0;
			}

			// プレス
			m_state[cnt] = aKeyState[cnt];
		}
	}
	else
	{
		// アクセス権取り直し
		m_inputDevice->Acquire();
	}
}

// [EOF]
