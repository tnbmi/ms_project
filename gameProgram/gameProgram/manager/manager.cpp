//*****************************************************************************
//
// メインマネージャー [manager.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "manager.h"
#include "..\common\safe.h"

#include "..\renderer\renderer.h"
#include "..\sound\sound.h"
#include "..\debugproc\debugproc.h"

#include "..\input\keyboard\keyboard.h"
#include "..\input\padX\padXManager.h"

#include "..\phase\title\title.h"
#include "..\phase\standby\standby.h"
#include "..\phase\game\game.h"
#include "..\phase\result\result.h"

#include "..\road\road.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Phase* Manager::m_nextPhase	= nullptr;

int Manager::m_scoreBlue = 600;
int Manager::m_scoreRed = 400;

//=============================================================================
// コンストラクタ
//=============================================================================
Manager::Manager(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_countFPS	= 0;

	m_phase		= nullptr;
	m_nextPhase	= nullptr;

	m_renderer	= nullptr;
	m_sound		= nullptr;
	m_debugproc	= nullptr;

	m_keyboard		= nullptr;
	m_padXManager	= nullptr;
	m_road			= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Manager::~Manager(void)
{
}

//=============================================================================
// 生成
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
// 初期化
//=============================================================================
bool Manager::Initialize(HINSTANCE hInstance, HWND hWnd, bool windowFlg)
{
	//----------------------------
	// レンダラー
	//----------------------------
	if(!Renderer::Create(&m_renderer, hWnd, windowFlg))
		return false;
	LPDIRECT3DDEVICE9 device = m_renderer->device();

	//----------------------------
	// 入力
	//----------------------------
	if(!Keyboard::Create(&m_keyboard, hInstance, hWnd))
		return false;
	if(!PadXManager::Create(&m_padXManager))
		return false;

	//----------------------------
	// サウンド
	//----------------------------
	if(!Sound::Create(&m_sound, hWnd))
		return false;

#ifdef _DEBUG
	//----------------------------
	// デバッグコメント
	//----------------------------
	if(!Debugproc::Create(&m_debugproc, device))
		return false;
	m_renderer->debugproc(m_debugproc);
	m_padXManager->debugproc(m_debugproc);
#endif
	//----------------------------
	// ローディング画面
	//----------------------------
	Road::Create( &m_road , device , this );

	//----------------------------
	// フェーズ
	//----------------------------
	// 生成
	m_phase = (Phase*)new Title(device);

	// 入力設定
	m_phase->padXManager(m_padXManager);
	m_phase->keyboard(m_keyboard);

#ifdef _DEBUG
	// デバッグ設定
	m_phase->debugproc(m_debugproc);
#endif

	// 初期化
	//if(!m_phase->Initialize())
	//	return false;
	m_road->Roading( m_phase);
	m_nextPhase = m_phase;
	m_renderer->phase(m_phase);
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Manager::Finalize(void)
{
	//----------------------------
	// フェーズ
	//----------------------------
	SafeFinalizeDelete(m_phase);

#ifdef _DEBUG
	//----------------------------
	// デバッグコメント
	//----------------------------
	SafeFinalizeDelete(m_debugproc);
#endif

	//----------------------------
	// サウンド
	//----------------------------
	Sound::StopAll();
	SafeFinalizeDelete(m_sound);

	//----------------------------
	// 入力
	//----------------------------
	SafeFinalizeDelete(m_keyboard);
	SafeFinalizeDelete(m_padXManager);

	//----------------------------
	// レンダラー
	//----------------------------
	SafeFinalizeDelete(m_renderer);

	//----------------------------
	// ローディング画面
	//----------------------------
	m_road->Finalize();
}

//=============================================================================
// 更新
//=============================================================================
bool Manager::Update(void)
{
#ifdef _DEBUG
	//----------------------------
	// デバッグ表示
	//----------------------------
	m_debugproc->Update();
	m_debugproc->PrintDebugProc("FPS:%d\n", m_countFPS);
#endif

	//----------------------------
	// 入力
	//----------------------------
	m_keyboard->Update();
	m_padXManager->Update();

	//----------------------------
	// レンダラー
	//----------------------------
	m_renderer->Update();

	//----------------------------
	// フェーズ切替
	//----------------------------
	if(m_nextPhase != m_phase)
	{
		// 現在フェーズを破棄
		//SafeFinalizeDelete(m_phase);

		// 入力設定
		m_nextPhase->padXManager(m_padXManager);
		m_nextPhase->keyboard(m_keyboard);

#ifdef _DEBUG
		// デバッグ設定
		m_nextPhase->debugproc(m_debugproc);
#endif

		// 次のフェーズを初期化・設定
		m_road->Roading( m_phase , m_nextPhase );
		//if(!m_nextPhase->Initialize())
		//	return false;
		m_phase = m_nextPhase;
		m_renderer->phase(m_phase);
	}
	return true;
}

//=============================================================================
// 描画
//=============================================================================
void Manager::Draw(void)
{
	//----------------------------
	// レンダラー
	//----------------------------
	m_renderer->Draw();
}

//=============================================================================
// FPS測定
//=============================================================================
void Manager::CalculateFPS(DWORD frameCnt, DWORD curTime, DWORD FPSLastTime)
{
	m_countFPS = frameCnt * 1000 / (curTime - FPSLastTime);
}

// [EOF]
