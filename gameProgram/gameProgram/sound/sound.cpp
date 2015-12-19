//*****************************************************************************
//
// 音源管理 [sound.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "sound.h"
#include "..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// パラメータ構造体定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct SOUND_PARAM
{
	char*	fileName;	// ファイル名
	bool	loop;		// ループするかどうか
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// サウンドファイルパス
const char* _sound_path = "./resources/sound";

//----------------------------
// サウンドデータ
//----------------------------
const SOUND_PARAM _sound_data[] =
{
	// BGM
	{"/bgm/TitleBGM01.wav", true},	// BGM_TITLE01
	{"/bgm/TitleBGM02.wav", true},	// BGM_TITLE02
	{"/bgm/TitleBGM03.wav", true},	// BGM_TITLE03
	{"/bgm/TitleBGM04.wav", true},	// BGM_TITLE04

	{"/bgm/StandbyBGM01.wav", true},	// BGM_STANDBY01
	{"/bgm/StandbyBGM02.wav", true},	// BGM_STANDBY02
	{"/bgm/StandbyBGM03.wav", true},	// BGM_STANDBY03
	{"/bgm/StandbyBGM04.wav", true},	// BGM_STANDBY04

	{"/bgm/GameBGM01.wav", true},	// BGM_GAME01
	{"/bgm/GameBGM02.wav", true},	// BGM_GAME02
	{"/bgm/GameBGM03.wav", true},	// BGM_GAME03
	{"/bgm/GameBGM04.wav", true},	// BGM_GAME04

	{"/bgm/ResultBgm.wav",true},

	// SE
	{"/se/fireworks.wav", false},	// SE_FIREWORKS
	{"/se/don.wav",false},//SE_DRUM
	{"/se/selectkanryouBlue.wav",false},//SE_SELECT_VOICEA
	{"/se/slectkanryouRED.wav",false},//SE_SELECT_VOICEB
	{"/se/iza.wav",false},//SE_VOICE_IZA
	{"/se/syoubu.wav",false},//SE_VOICE_BATTLE
	{"/se/dodon.wav",false},
	{"/se/syouriBlue.wav",false},
	{"/se/syouriRed.wav",false},
	{"/se/moriagariMid.wav",false},
	{"/se/attackBlue1.wav",false},
	{"/se/attackBlue2.wav",false},
	{"/se/attackBlue3.wav",false},
	{"/se/attackRed1.wav",false},
	{"/se/attackRed2.wav",false},
	{"/se/attackRed3.wav",false},
	{"/se/missBlue.wav",false},
	{"/se/missRed.wav",false},
	{"/se/douzioshi.wav",false},
	{"/se/nebtaDouziBlue.wav",false},
	{"/se/nebtaDouziRed.wav",false},
	{"/se/hazime.wav",false},
	{"/se/soremade.wav",false},
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
IXAudio2SourceVoice* Sound::m_sourceVoice[Sound::SOUND_MAX]	= {nullptr};
BYTE*				 Sound::m_dataAudio[Sound::SOUND_MAX]	= {nullptr};
DWORD				 Sound::m_sizeAudio[Sound::SOUND_MAX]	= {0};

//=============================================================================
// コンストラクタ
//=============================================================================
Sound::Sound(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_xAudio2		 = nullptr;
	m_masteringVoice = nullptr;

	for(int cnt = 0; cnt <SOUND_MAX; ++cnt)
	{
		m_sourceVoice[cnt]	= nullptr;
		m_dataAudio[cnt]	= nullptr;
		m_sizeAudio[cnt]	= 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
Sound::~Sound(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Sound::Create(Sound** outPointer, HWND hWnd)
{
	Sound* pointer = new Sound();
	if(!pointer->Initialize(hWnd))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Sound::Initialize(HWND hWnd)
{
	HRESULT hr;

	//----------------------------
	// COMライブラリの初期化
	//----------------------------
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//----------------------------
	// XAudio2オブジェクトの作成
	//----------------------------
	hr = XAudio2Create(&m_xAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();
		return false;
	}

	//----------------------------
	// マスターボイスの生成
	//----------------------------
	hr = m_xAudio2->CreateMasteringVoice(&m_masteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// XAudio2オブジェクトの開放
		SafeRelease(m_xAudio2);

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}

	//----------------------------
	// サウンドデータの初期化
	//----------------------------
	DWORD	dwChunkSize		= 0;
	DWORD	dwChunkPosition	= 0;
	DWORD	dwFiletype		= 0;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER		 buffer;
	HANDLE	file;
	char	str[512];

	for(int cnt = 0; cnt < SOUND_MAX; ++cnt)
	{
		dwChunkSize		= 0;
		dwChunkPosition	= 0;
		dwFiletype		= 0;

		//----------------------------
		// バッファのクリア
		//----------------------------
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		//----------------------------
		// パス連結
		//----------------------------
		memset(str, 0, sizeof(str));
		strcpy_s(str, _sound_path);
		strcat_s(str, sizeof(str), _sound_data[cnt].fileName);

		//----------------------------
		// サウンドデータファイルの生成
		//----------------------------
		file = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if(file == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			//return HRESULT_FROM_WIN32(GetLastError());
			return false;
		}
		if(SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			//return HRESULT_FROM_WIN32(GetLastError());
			return false;
		}

		//----------------------------
		// WAVEファイルのチェック
		//----------------------------
		hr = CheckChunk(file, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(file, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return false;
		}

		//----------------------------
		// フォーマットチェック
		//----------------------------
		hr = CheckChunk(file, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(file, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}

		//----------------------------
		// オーディオデータ読み込み
		//----------------------------
		hr = CheckChunk(file, 'atad', &m_sizeAudio[cnt], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		m_dataAudio[cnt] = (BYTE*)malloc(m_sizeAudio[cnt]);
		hr = ReadChunkData(file, m_dataAudio[cnt], m_sizeAudio[cnt], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}

		//----------------------------
		// ソースボイスの生成
		//----------------------------
		hr = m_xAudio2->CreateSourceVoice(&m_sourceVoice[cnt], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return false;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes	= m_sizeAudio[cnt];
		buffer.pAudioData	= m_dataAudio[cnt];
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.LoopCount	= 0;

		//----------------------------
		// オーディオバッファの登録
		//----------------------------
		m_sourceVoice[cnt]->SubmitSourceBuffer(&buffer);
	}

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Sound::Finalize(void)
{
	//----------------------------
	// 一時停止
	//----------------------------
	for(int cnt = 0; cnt < SOUND_MAX; ++cnt)
	{
		if(m_sourceVoice[cnt])
		{
			// 一時停止
			m_sourceVoice[cnt]->Stop(0);
	
			// ソースボイスの破棄
			m_sourceVoice[cnt]->DestroyVoice();
			m_sourceVoice[cnt] = nullptr;
	
			// オーディオデータの開放
			free(m_dataAudio[cnt]);
			m_dataAudio[cnt] = nullptr;
		}
	}

	//----------------------------
	// マスターボイスの破棄
	//----------------------------
	m_masteringVoice->DestroyVoice();
	m_masteringVoice = nullptr;

	// XAudio2オブジェクトの開放
	SafeRelease(m_xAudio2);

	//----------------------------
	// COMライブラリの終了処理
	//----------------------------
	CoUninitialize();
}

//=============================================================================
// 再生
//=============================================================================
void Sound::Play(SOUND_TABLE id)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes	= m_sizeAudio[id];
	buffer.pAudioData	= m_dataAudio[id];
	buffer.Flags		= XAUDIO2_END_OF_STREAM;
	buffer.LoopCount	= 0;

	//----------------------------
	// ループ処理
	//----------------------------
	if(_sound_data[id].loop)
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//----------------------------
	// 状態取得
	//----------------------------
	m_sourceVoice[id]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_sourceVoice[id]->Stop(0);

		// オーディオバッファの削除
		m_sourceVoice[id]->FlushSourceBuffers();
	}

	//----------------------------
	// オーディオバッファの登録
	//----------------------------
	m_sourceVoice[id]->SubmitSourceBuffer(&buffer);

	//----------------------------
	// 再生
	//----------------------------
	m_sourceVoice[id]->Start(0);
}

//=============================================================================
// 停止
//=============================================================================
void Sound::Stop(SOUND_TABLE id)
{
	XAUDIO2_VOICE_STATE xa2state;

	//----------------------------
	// 状態取得
	//----------------------------
	m_sourceVoice[id]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_sourceVoice[id]->Stop(0);

		// オーディオバッファの削除
		m_sourceVoice[id]->FlushSourceBuffers();
	}
}

//=============================================================================
// 全停止
//=============================================================================
void Sound::StopAll(void)
{
	//----------------------------
	// 一時停止
	//----------------------------
	for(int cnt = 0; cnt < SOUND_MAX; ++cnt)
	{
		if(m_sourceVoice[cnt])
			// 一時停止
			m_sourceVoice[cnt]->Stop(0);
	}
}

//=============================================================================
// チャンク確認
//=============================================================================
HRESULT Sound::CheckChunk(HANDLE file, DWORD format, DWORD *chunkSize, DWORD* chunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());

	while(hr == S_OK)
	{
		if(ReadFile(file, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
			// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());

		if(ReadFile(file, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
			// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize	= dwChunkDataSize;
			dwChunkDataSize	= 4;
			if(ReadFile(file, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
				// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if(SetFilePointer(file, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*chunkSize			= dwChunkDataSize;
			*chunkDataPosition	= dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
			return S_FALSE;
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータ読み込み
//=============================================================================
HRESULT Sound::ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(file, bufferOffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());

	if(ReadFile(file, buffer, bufferSize, &dwRead, nullptr) == 0)
		// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

// EOF
