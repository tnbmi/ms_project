//*****************************************************************************
//
// �����Ǘ� [sound.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_SOUND_H_
#define MY_SOUND_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Sound
{
public:
	enum SOUND_TABLE
	{
		BGM_A,
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

	IXAudio2*				m_xAudio2;					// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice*	m_masteringVoice;			// �}�X�^�[�{�C�X

	static IXAudio2SourceVoice*	m_sourceVoice[SOUND_MAX];		// �\�[�X�{�C�X
	static BYTE*				m_dataAudio[SOUND_MAX];		// �I�[�f�B�I�f�[�^
	static DWORD				m_sizeAudio[SOUND_MAX];		// �I�[�f�B�I�f�[�^�T�C�Y

};

//=============================================================================
#endif

// EOF
