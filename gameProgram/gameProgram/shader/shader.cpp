//*****************************************************************************
//
// �V�F�[�_�[�Ǘ� [shader.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "shader.h"
#include "..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �V�F�[�_�[�f�[�^
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �V�F�[�_�[�t�@�C���p�X
const char* _shader_path = "../resources/shader";

//----------------------------
// �o�[�e�b�N�X�V�F�[�_
//----------------------------
// �i���o�����O
enum VS_TABLE
{
	VS_NONE	 = -1,
	VS_2D	 = 0,
	VS_LIGHT,
	VS_FBX,
	VS_INS,
};

// �t�@�C���p�X
const char* _vsName[] =
{
	{"/twoDimensionsVS.hlsl"},
	{"/lightVS.hlsl"},
	{"/fbxVS.hlsl"},
	{"/instancingVS.hlsl"}
};

//----------------------------
// �s�N�Z���V�F�[�_
//----------------------------
// �i���o�����O
enum PS_TABLE
{
	PS_NONE = -1,
	PS_2D = 0,
	PS_BASIC,
	PS_FBX,
	PS_INS
};

// �t�@�C���p�X
const char* _psName[] =
{
	{"/twoDimensionsPS.hlsl"},
	{"/basicPS.hlsl"},
	{"/fbxPS.hlsl"},
	{"/instancingPS.hlsl"}
};

//----------------------------
// �p�^�[���̃Z�b�g
//----------------------------
// ID�Z�b�g
struct SHADER_SET
{
	VS_TABLE vscID;
	PS_TABLE pscID;
};

// �p�^�[���Z�b�g���e
const SHADER_SET _shader_set[Shader::PATTERN_MAX] =
{
	{VS_NONE,	PS_NONE},		// NONE
	{VS_LIGHT,	PS_BASIC},		// LIGHT
	{VS_FBX,	PS_FBX},		// FBX
	{VS_INS,	PS_INS},
	{VS_2D,		PS_2D}			// 2D
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Shader::Shader(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_device = device;

	m_vsSize = 0;
	m_vertexShader		= nullptr;
	m_vsc	= nullptr;

	m_psSize = 0;
	m_pixelShader		= nullptr;
	m_psc	= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Shader::~Shader(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Shader::Create(Shader** outPointer, LPDIRECT3DDEVICE9 device)
{
	Shader* pointer = new Shader(device);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Shader::Initialize(void)
{
	HRESULT		 hr;
	LPD3DXBUFFER err	= 0;
	LPD3DXBUFFER code	= 0;
	char		 str[512];

	//------------------------------
	// �o�[�e�b�N�X�V�F�[�_�[
	//------------------------------
	m_vsSize			= sizeof(_vsName) / sizeof(char*);
	m_vertexShader		= new LPDIRECT3DVERTEXSHADER9[m_vsSize];
	m_vsc	= new LPD3DXCONSTANTTABLE[m_vsSize];

	for(int cnt = 0; cnt < m_vsSize; ++cnt)
	{
		// �p�X�A��
		memset(str, 0, sizeof(str));
		strcpy_s(str, _shader_path);
		strcat_s(str, sizeof(str), _vsName[cnt]);

		// �V�F�[�_�[�t�@�C���ǂݍ���
		hr = D3DXCompileShaderFromFile(str, NULL, NULL,
										"VS", "vs_3_0", 0, &code, &err, &m_vsc[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
			err->Release();
			return false;
		}

		// �ݒ�
		hr = m_device->CreateVertexShader((DWORD*)code->GetBufferPointer(), &m_vertexShader[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, "FAILED", "CreateVertexShader", MB_OK);
			return false;
		}
	}

	//------------------------------
	// �s�N�Z���V�F�[�_�[
	//------------------------------
	m_psSize			= sizeof(_psName) / sizeof(char*);
	m_pixelShader		= new LPDIRECT3DPIXELSHADER9[m_psSize];
	m_psc	= new LPD3DXCONSTANTTABLE[m_psSize];

	for(int cnt = 0; cnt < m_psSize; ++cnt)
	{
		// �p�X�A��
		memset(str, 0, sizeof(str));
		strcpy_s(str, _shader_path);
		strcat_s(str, sizeof(str), _psName[cnt]);

		// �V�F�[�_�[�t�@�C���ǂݍ���
		hr = D3DXCompileShaderFromFile(str, NULL, NULL,
										"PS", "ps_3_0", 0, &code, &err,&m_psc[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, (LPCSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
			err->Release();
			return false;
		}

		// �ݒ�
		hr = m_device->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_pixelShader[cnt]);
		if(FAILED(hr))
		{
			MessageBox(NULL, "FAILED", "CreatePixelShader", MB_OK);
			return false;
		}
	}

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Shader::Finalize(void)
{
	// �o�[�e�b�N�X
	for(int cnt = 0; cnt < m_vsSize; ++cnt)
	{
		SafeRelease(m_vertexShader[cnt]);
		SafeRelease(m_vsc[cnt]);
	}
	SafeDeleteArray(m_vertexShader);
	SafeDeleteArray(m_vsc);

	// �s�N�Z��
	for(int cnt = 0; cnt < m_psSize; ++cnt)
	{
		SafeRelease(m_pixelShader[cnt]);
		SafeRelease(m_psc[cnt]);
	}
	SafeDeleteArray(m_pixelShader);
	SafeDeleteArray(m_psc);
	
}

//=============================================================================
// �V�F�[�_�[�ݒ�
//=============================================================================
void Shader::SetShader(LPD3DXCONSTANTTABLE* outVSC, LPD3DXCONSTANTTABLE* outPSC, PATTERN patternId)
{
	//----------------------------
	// �o�[�e�b�N�X�V�F�[�_�[�ݒ�
	//----------------------------
	VS_TABLE vsID = _shader_set[patternId].vscID;

	if(vsID >= 0)
	{
		// �o�[�e�b�N�X�V�F�[�_�[
		m_device->SetVertexShader(m_vertexShader[vsID]);

		// �o�[�e�b�N�X�V�F�[�_�[�e�[�u��
		*outVSC = m_vsc[vsID];
	}
	else
	{
		// �o�[�e�b�N�X�V�F�[�_�[
		m_device->SetVertexShader(nullptr);

		// �o�[�e�b�N�X�V�F�[�_�[�e�[�u��
		*outVSC = nullptr;
	}

	//----------------------------
	// �s�N�Z���V�F�[�_�[�ݒ�
	//----------------------------
	PS_TABLE psID = _shader_set[patternId].pscID;

	if(psID >= 0)
	{
		// �s�N�Z���V�F�[�_�[
		m_device->SetPixelShader(m_pixelShader[psID]);

		// �s�N�Z���V�F�[�_�[�e�[�u��
		*outPSC = m_psc[psID];
	}
	else
	{
		// �s�N�Z���V�F�[�_�[
		m_device->SetPixelShader(nullptr);

		// �s�N�Z���V�F�[�_�[�e�[�u��
		*outPSC = nullptr;
	}
}

// EOF
