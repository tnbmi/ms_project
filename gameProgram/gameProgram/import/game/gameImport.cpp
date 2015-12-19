//*****************************************************************************
//
// �Q�[���̃C���|�[�g [gameImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "gameImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �e�N�X�`��
//----------------------------
const char* _game_texture_name[] =
{
	nullptr,

	"/stones.bmp",
	"/stones_normal.bmp",
	"/sky.jpg",

	"/COMMAND_TEX.png",
	"/blue.png",
	"/red.png",
	"/commandfram00.png",
	"/commandfram01.png",
	"/number.png",
	"/num_yellow.png",
	"/num_white.png",
	"/GameUI.png",
	"/CutinRed.png",
	"/Cutinblue.png",
	"/ougi.png",
	"/effect.jpg"
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
GameImport::GameImport(void) : Import(TEX_MAX)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	// �e�N�X�`��
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_texture[cnt] = nullptr;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
GameImport::~GameImport(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool GameImport::Create(GameImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	GameImport* pointer = new GameImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool GameImport::Initialize(LPDIRECT3DDEVICE9 device)
{
	HRESULT	hr;
	char	str[512];

	//----------------------------
	// �e�N�X�`��
	//----------------------------
	for(int cnt = 1; cnt < TEX_MAX; ++cnt)
	{
		// �p�X�A��
		memset(str, 0, sizeof(str));
		strcpy_s(str, TEXTURE_PATH);
		strcat_s(str, sizeof(str), _game_texture_name[cnt]);

		// �e�N�X�`���ǂݍ���
		hr = D3DXCreateTextureFromFile(device, str, &m_texture[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, "�e�N�X�`������", "D3DXCreateTextureFromFile", MB_OK);
		}
	}

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void GameImport::Finalize(void)
{
	//----------------------------
	// �e�N�X�`��
	//----------------------------
	for(int cnt = 1; cnt < TEX_MAX; cnt++)
	{
		// �e�N�X�`���̊J��
		SafeRelease(m_texture[cnt]);
	}
}

// EOF
