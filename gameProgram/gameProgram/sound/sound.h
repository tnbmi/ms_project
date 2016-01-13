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
		BGM_TITLE03,

		BGM_STANDBY02,

		BGM_GAME01,

		BGM_RESULT01,

		// SE
		SE_FIREWORKS,
		SE_DRUM,
		SE_SELECT_VOICEA,
		SE_SELECT_VOICEB,
		SE_SELECT_VOICE_IZA,
		SE_SELECT_VOICE_BATTLE,
		SE_DRUM_DODON,
		SE_MORIAGARI,
		SE_SYOURI_BLUE,
		SE_SYOURI_RED,
		SE_ATTACK_BLUE1,
		SE_ATTACK_BLUE2,
		SE_ATTACK_BLUE3,
		SE_ATTACK_RED1,
		SE_ATTACK_RED2,
		SE_ATTACK_RED3,
		SE_FAIL_BLUE,
		SE_FAIL_RED,
		SE_SAME,
		SE_SAME_NEB_BLUE,
		SE_SAME_NEB_RED,
		SE_START_VOICE,
		SE_END_VOICE,
		SE_KAIMAKU,
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
