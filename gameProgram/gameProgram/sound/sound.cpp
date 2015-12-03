//*****************************************************************************
//
// �����Ǘ� [sound.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "sound.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �p�����[�^�\���̒�`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct SOUND_PARAM
{
	char*	fileName;	// �t�@�C����
	bool	loop;		// ���[�v���邩�ǂ���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �T�E���h�t�@�C���p�X
const char* _sound_path = "../resources/sound";

//----------------------------
// �T�E���h�f�[�^
//----------------------------
const SOUND_PARAM _sound_data[] =
{
	{"/bgm/kiseki.wav", true}	// BGM_A
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
IXAudio2SourceVoice* Sound::m_sourceVoice[Sound::SOUND_MAX]	= {nullptr};
BYTE*				 Sound::m_dataAudio[Sound::SOUND_MAX]	= {nullptr};
DWORD				 Sound::m_sizeAudio[Sound::SOUND_MAX]	= {0};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Sound::Sound(void)
{
	//----------------------------
	// �����o�[������
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
// �f�X�g���N�^
//=============================================================================
Sound::~Sound(void)
{
}

//=============================================================================
// ����
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
// ������
//=============================================================================
bool Sound::Initialize(HWND hWnd)
{
	HRESULT hr;

	//----------------------------
	// COM���C�u�����̏�����
	//----------------------------
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//----------------------------
	// XAudio2�I�u�W�F�N�g�̍쐬
	//----------------------------
	hr = XAudio2Create(&m_xAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return false;
	}

	//----------------------------
	// �}�X�^�[�{�C�X�̐���
	//----------------------------
	hr = m_xAudio2->CreateMasteringVoice(&m_masteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_xAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_xAudio2->Release();
			m_xAudio2 = nullptr;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return false;
	}

	//----------------------------
	// �T�E���h�f�[�^�̏�����
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
		// �o�b�t�@�̃N���A
		//----------------------------
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		//----------------------------
		// �p�X�A��
		//----------------------------
		memset(str, 0, sizeof(str));
		strcpy_s(str, _sound_path);
		strcat_s(str, sizeof(str), _sound_data[cnt].fileName);

		//----------------------------
		// �T�E���h�f�[�^�t�@�C���̐���
		//----------------------------
		file = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if(file == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			//return HRESULT_FROM_WIN32(GetLastError());
			return false;
		}
		if(SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			//return HRESULT_FROM_WIN32(GetLastError());
			return false;
		}

		//----------------------------
		// WAVE�t�@�C���̃`�F�b�N
		//----------------------------
		hr = CheckChunk(file, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(file, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return false;
		}

		//----------------------------
		// �t�H�[�}�b�g�`�F�b�N
		//----------------------------
		hr = CheckChunk(file, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(file, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}

		//----------------------------
		// �I�[�f�B�I�f�[�^�ǂݍ���
		//----------------------------
		hr = CheckChunk(file, 'atad', &m_sizeAudio[cnt], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		m_dataAudio[cnt] = (BYTE*)malloc(m_sizeAudio[cnt]);
		hr = ReadChunkData(file, m_dataAudio[cnt], m_sizeAudio[cnt], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}

		//----------------------------
		// �\�[�X�{�C�X�̐���
		//----------------------------
		hr = m_xAudio2->CreateSourceVoice(&m_sourceVoice[cnt], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return false;
		}

		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes	= m_sizeAudio[cnt];
		buffer.pAudioData	= m_dataAudio[cnt];
		buffer.Flags		= XAUDIO2_END_OF_STREAM;
		buffer.LoopCount	= 0;

		//----------------------------
		// �I�[�f�B�I�o�b�t�@�̓o�^
		//----------------------------
		m_sourceVoice[cnt]->SubmitSourceBuffer(&buffer);
	}

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Sound::Finalize(void)
{
	//----------------------------
	// �ꎞ��~
	//----------------------------
	for(int cnt = 0; cnt < SOUND_MAX; ++cnt)
	{
		if(m_sourceVoice[cnt])
		{
			// �ꎞ��~
			m_sourceVoice[cnt]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_sourceVoice[cnt]->DestroyVoice();
			m_sourceVoice[cnt] = nullptr;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_dataAudio[cnt]);
			m_dataAudio[cnt] = nullptr;
		}
	}

	//----------------------------
	// �}�X�^�[�{�C�X�̔j��
	//----------------------------
	m_masteringVoice->DestroyVoice();
	m_masteringVoice = nullptr;

	if(m_xAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̉��
		m_xAudio2->Release();
		m_xAudio2 = nullptr;
	}

	//----------------------------
	// COM���C�u�����̏I������
	//----------------------------
	CoUninitialize();
}

//=============================================================================
// �Đ�
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
	// ���[�v����
	//----------------------------
	if(_sound_data[id].loop)
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	//----------------------------
	// ��Ԏ擾
	//----------------------------
	m_sourceVoice[id]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_sourceVoice[id]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_sourceVoice[id]->FlushSourceBuffers();
	}

	//----------------------------
	// �I�[�f�B�I�o�b�t�@�̓o�^
	//----------------------------
	m_sourceVoice[id]->SubmitSourceBuffer(&buffer);

	//----------------------------
	// �Đ�
	//----------------------------
	m_sourceVoice[id]->Start(0);
}

//=============================================================================
// ��~
//=============================================================================
void Sound::Stop(SOUND_TABLE id)
{
	XAUDIO2_VOICE_STATE xa2state;

	//----------------------------
	// ��Ԏ擾
	//----------------------------
	m_sourceVoice[id]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_sourceVoice[id]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_sourceVoice[id]->FlushSourceBuffers();
	}
}

//=============================================================================
// �S��~
//=============================================================================
void Sound::StopAll(void)
{
	//----------------------------
	// �ꎞ��~
	//----------------------------
	for(int cnt = 0; cnt < SOUND_MAX; ++cnt)
	{
		if(m_sourceVoice[cnt])
			// �ꎞ��~
			m_sourceVoice[cnt]->Stop(0);
	}
}

//=============================================================================
// �`�����N�m�F
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
		// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());

	while(hr == S_OK)
	{
		if(ReadFile(file, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
			// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());

		if(ReadFile(file, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
			// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize	= dwChunkDataSize;
			dwChunkDataSize	= 4;
			if(ReadFile(file, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
				// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if(SetFilePointer(file, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�ǂݍ���
//=============================================================================
HRESULT Sound::ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(file, bufferOffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());

	if(ReadFile(file, buffer, bufferSize, &dwRead, nullptr) == 0)
		// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

// EOF
