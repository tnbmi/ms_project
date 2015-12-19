//*****************************************************************************
//
// ���C���̃C���|�[�g [mainImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "mainImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �e�N�X�`��
//----------------------------
const char* _main_texture_name[] =
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
	"/MainUI.png",
	"/B�@.png",
	"/B�A.png",
	"/B�B.png",
	"/B�C.png",
	"/B�D.png",
	"/B�E.png",
	"/B�F.png",
	"/B�G.png",
	"/B�H.png",
	"/R�@.png",
	"/R�A.png",
	"/R�B.png",
	"/R�C.png",
	"/R�D.png",
	"/R�E.png",
	"/R�F.png",
	"/R�G.png",
	"/R�H.png",
	"/CutinRed.png",
	"/Cutinblue.png",
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
MainImport::MainImport(void) : Import(TEX_MAX)
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
MainImport::~MainImport(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool MainImport::Create(MainImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	MainImport* pointer = new MainImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool MainImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _main_texture_name[cnt]);

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
void MainImport::Finalize(void)
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
