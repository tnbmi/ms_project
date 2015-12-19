//*****************************************************************************
//
// �����̃C���|�[�g [standbyImport.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "standbyImport.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �e�N�X�`��
//----------------------------
const char* _standby_texture_name[] =
{
	nullptr,
	
	"/Select_Back.png",
	"/Select_Mid.png",
	"/Select_Front.png",
	"/Select_ZyunbiOK_Blue.png",
	"/Select_ZyunbiOK_Red.png",
	"/Select_Syoubu.png",
	"/Select_Blue.png",
	"/Select_RedScroll.png",
	"/B�@.png",
	"/B�H.png",
	"/R�@.png",
	"/R�H.png"

};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
StandbyImport::StandbyImport(void) : Import(TEX_MAX)
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
StandbyImport::~StandbyImport(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool StandbyImport::Create(StandbyImport** outPointer, LPDIRECT3DDEVICE9 device)
{
	StandbyImport* pointer = new StandbyImport();
	if(!pointer->Initialize(device))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool StandbyImport::Initialize(LPDIRECT3DDEVICE9 device)
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
		strcat_s(str, sizeof(str), _standby_texture_name[cnt]);

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
void StandbyImport::Finalize(void)
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
