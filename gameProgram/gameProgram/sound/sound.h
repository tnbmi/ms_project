//*****************************************************************************
//
// 音源管理 [sound.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_SOUND_H_
#define MY_SOUND_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Sound
{
public:
	enum SOUND_TABLE
	{
		// BGM
		BGM_TITLE01,
		BGM_TITLE02,
		BGM_TITLE03,
		BGM_TITLE04,

		BGM_STANDBY01,
		BGM_STANDBY02,
		BGM_STANDBY03,
		BGM_STANDBY04,

		BGM_GAME01,
		BGM_GAME02,
		BGM_GAME03,
		BGM_GAME04,

		// SE
		SE_FIREWORKS,
		SOUND_MAX
	};

	Sound(void);
	~Sound(void);

	static bool Create(Sound** outPointer, HWND hWnd);
	bool Initialize(HWND hWnd);
	void Finalize(void);

	static void Play(SOUND_TABLE id);
	static void Stop(SOUND_TABLE id);
	static void StopAll(void);

private:
	HRESULT CheckChunk(HANDLE file, DWORD format, DWORD *chunkSize, DWORD* chunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);

	IXAudio2*				m_xAudio2;					// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice*	m_masteringVoice;			// マスターボイス

	static IXAudio2SourceVoice*	m_sourceVoice[SOUND_MAX];		// ソースボイス
	static BYTE*				m_dataAudio[SOUND_MAX];		// オーディオデータ
	static DWORD				m_sizeAudio[SOUND_MAX];		// オーディオデータサイズ

};

//=============================================================================
#endif

// EOF
