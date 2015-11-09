//*****************************************************************************
//
// 描画機構 [renderer.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "renderer.h"
#include "..\common\safe.h"

#include "..\debugproc\debugproc.h"
#include "..\phase\phase.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Renderer::Renderer(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_device	= nullptr;
	m_debugproc	= nullptr;
	m_phase		= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Renderer::~Renderer(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Renderer::Create(Renderer** outPointer, HWND hWnd, bool windowFlg)
{
	Renderer* pointer = new Renderer();
	if(!pointer->Initialize(hWnd, windowFlg))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Renderer::Initialize(HWND hWnd, bool windowFlg)
{
	HRESULT					hResult;		// 初期化判定
	LPDIRECT3D9				object;			// Direct3Dオブジェクト
	D3DDISPLAYMODE			displayMode;	// ディスプレイモード
	D3DPRESENT_PARAMETERS	presenParam;	// デバイスのプレゼンテーションパラメータ

	//----------------------------
	// Direct3Dオブジェクトの作成
	//----------------------------
	object = Direct3DCreate9(D3D_SDK_VERSION);
	if(object == nullptr)
		return false;

	//----------------------------
	// 現在のディスプレイモードを取得
	//----------------------------
	hResult = object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
	if(FAILED(hResult))
	{
		MessageBox(NULL, "Renderer", "ディスプレイモード取得時にエラーが発生しました。", MB_OK);
		return false;
	}

	//----------------------------
	// デバイスのプレゼンテーションパラメータの設定
	//----------------------------
	ZeroMemory(&presenParam, sizeof(presenParam));					// ワークをゼロクリア
	presenParam.BackBufferCount			= 1;						// バックバッファの数
	presenParam.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	presenParam.BackBufferHeight		= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	presenParam.BackBufferFormat		= displayMode.Format;		// バックバッファフォーマットはディスプレイモードに合わせて使う
	presenParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	presenParam.Windowed				= windowFlg;				// ウィンドウモード
	presenParam.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	presenParam.AutoDepthStencilFormat	= D3DFMT_D24S8;				// デプスバッファとして16bitを使う(高品質はD24S8)

	//----------------------------
	// ウィンドウ分岐
	//----------------------------
	if(windowFlg)
	{// ウィンドウモード
		presenParam.FullScreen_RefreshRateInHz	= 0;								// リフレッシュレート
		presenParam.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		presenParam.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		presenParam.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	//----------------------------
	// デバイスオブジェクト生成
	//----------------------------
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	hResult = object->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&presenParam,
									&m_device);
	if(FAILED(hResult))
	{// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		hResult = object->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&presenParam,
										&m_device);
		if(FAILED(hResult))
		{// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			hResult = object->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&presenParam,
											&m_device);
			if(FAILED(hResult))
			{
				MessageBox(NULL, "Renderer", "デバイスオブジェクト生成に失敗しました。", MB_OK);
				return false;
			}
		}
	}

	//----------------------------
	// Direct3Dオブジェクト開放
	//----------------------------
	SafeRelease(object);

	//----------------------------
	// デバイス設定
	//----------------------------
	// レンダーステートパラメータ
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータ
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定
	
	// テクスチャステージステート
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// αテスト数値
	m_device->SetRenderState(D3DRS_ALPHAREF, 0x55);					// 不透明にする値の設定
	m_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// 比較方法

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Renderer::Finalize(void)
{
	//----------------------------
	// フォーマット
	//----------------------------
	SafeRelease(m_decl);

	//----------------------------
	// デバイスオブジェクトの開放
	//----------------------------
	SafeRelease(m_device);
}

//=============================================================================
// 更新
//=============================================================================
void Renderer::Update(void)
{
	//----------------------------
	// フェーズ描画
	//----------------------------
	m_phase->Update();
}

//=============================================================================
// 描画
//=============================================================================
void Renderer::Draw(void)
{
	//----------------------------
	// Direct3Dによる描画
	//----------------------------
	if(SUCCEEDED(m_device->BeginScene()))	//開始
	{
		//----------------------------
		// フェーズ描画
		//----------------------------
		m_phase->Draw();

#ifdef _DEBUG
		//----------------------------
		// デバッグ描画
		//----------------------------
		m_debugproc->Draw();
#endif

		m_device->EndScene();	// 終了
	}

	//----------------------------
	// バックバッファとフロントバッファの入れ替え
	//----------------------------
	m_device->Present(nullptr, nullptr, nullptr, nullptr);
}

// [EOF]
