//*****************************************************************************
//
// メイン [main.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "..\common\safe.h"

#include "window\window.h"
#include "..\manager\manager.h"

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//----------------------------
	// メモリーリークチェック
	//----------------------------
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//----------------------------
	// 未使用宣言
	//----------------------------
	UNREFERENCED_PARAMETER(prevInstance);	// 無くても良いけど、警告が出る
	UNREFERENCED_PARAMETER(cmdLine);		// 無くても良いけど、警告が出る

	//----------------------------
	// ウィンドウ生成
	//----------------------------
	Window window;
	if(!window.Initialize(hInstance, cmdShow))
		return -1;

	//----------------------------
	// フルスクリーン設定
	//----------------------------
	bool windowFlg = true;
	if(MessageBox(nullptr, "フルスクリーンで起動しますか","フルスクリーン設定",MB_YESNO)==IDYES)
		//フルスクリーンで初期化処理(ウィンドウを作成してから行う)
		windowFlg = FALSE;
	else
		//通常の 初期化処理(ウィンドウを作成してから行う)
		windowFlg = TRUE;

	//----------------------------
	// マネージャー生成
	//----------------------------
	Manager* manager = nullptr;
	if(!Manager::Create(&manager, hInstance, window.hWnd(), windowFlg))
		return -1;

	//----------------------------
	// フレームカウント
	//----------------------------
	DWORD execLastTime;	// 処理時刻
	DWORD FPSLastTime;	// FPS測定時刻
	DWORD curTime;		// 現在時刻
	DWORD frameCnt;		// フレームカウント

	timeBeginPeriod(1);				// 分解能を設定
	execLastTime = timeGetTime();
	FPSLastTime	 = execLastTime;	// システム時刻をミリ秒単位で取得
	curTime		 =	 0;				// 現在時刻とカウントの初期化
	frameCnt	 = curTime;

	//----------------------------
	// メッセージループ
	//----------------------------
	MSG msg;

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // メッセージを取得しなかった場合"0"を返す
		{
			if(msg.message == WM_QUIT)
			{// ループ終了
				break;
			}
			else
			{// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			curTime = timeGetTime();			// システム時刻を取得
			if((curTime - FPSLastTime) >= 500)
			{// FPSを測定
				manager->CalculateFPS(frameCnt, curTime, FPSLastTime);
				FPSLastTime = curTime;
				frameCnt = 0;
			}

			if((curTime - execLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				execLastTime = curTime;	// 処理した時刻を保存
				++frameCnt;

				manager->Update();
				manager->Draw();
			}
		}
	}

	//----------------------------
	// 終了処理
	//----------------------------
	// フレームカウント
	timeEndPeriod(1);

	// マネージャー
	SafeFinalizeDelete(manager);

	return (int)msg.wParam;
}

// EOF
