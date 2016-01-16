//*****************************************************************************
//
// ���U���g�̃C���|�[�g [resultImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "resultImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �e�N�X�`��
//----------------------------
const char* _result_texture_name[] =
{
	nullptr,

	"/ground1.png",
	"/BG1.png",
	"/sky.jpg",

	"/number.png",
	"/syouriscrollRed.png",
	"/syouriscrollBlue.png",
	"/kekkaUI.png",
	"/redougi.png",
	"/blueougi.png",
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ResultImport::ResultImport(void) : Import(TEX_MAX)
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
ResultImport::~ResultImport(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool ResultImport::Create(ResultImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	ResultImport* pointer = new ResultImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool ResultImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _result_texture_name[cnt]);

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
void ResultImport::Finalize(void)
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
