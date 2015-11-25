//*****************************************************************************
//
// �`��Ǘ����X�g�}�l�[�W���[ [drawList.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "drawListManager.h"
#include "..\..\common\safe.h"

#include "drawList.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
DrawListManager::DrawListManager(LPDIRECT3DDEVICE9 device, Shader* shader)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_device = device;
	m_shader = shader;

	// ���X�g
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
		m_drawList[cnt] = nullptr;

	// 2D���_�錾
	HRESULT hr;
	/*D3DVERTEXELEMENT9 velement2D[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT,	0},		// ���W
		{0, 12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},		// �F
		{0, 16, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},		// �e�N�X�`��
		D3DDECL_END(),
	}

	hr = m_device->CreateVertexDeclaration(&velement2D[0], &m_decl2D);
	if(FAILED(hr))
		MessageBox(NULL, "2D���_�錾���s", "CreateVertexDeclaration", MB_OK);;*/

	// 3D���_�錾
	D3DVERTEXELEMENT9 velement3D[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},		// ���W
		{0, 12, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},		// �@��
		//{0, 24, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},		// �F
		{0, 24/*28*/, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},		// �e�N�X�`��
		D3DDECL_END(),
	};

	hr = m_device->CreateVertexDeclaration(&velement3D[0], &m_decl3D);
	if(FAILED(hr))
		MessageBox(NULL, "3D���_�錾���s", "CreateVertexDeclaration", MB_OK);

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
DrawListManager::~DrawListManager(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool DrawListManager::Create(DrawListManager** outPointer, LPDIRECT3DDEVICE9 device, Shader* shader)
{
	DrawListManager* pointer = new DrawListManager(device, shader);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool DrawListManager::Initialize(void)
{
	//----------------------------
	// �`��Ǘ����X�g������
	//----------------------------
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
		if(!DrawList::Create(&m_drawList[cnt], (Shader::PATTERN)cnt))
			return false;

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void DrawListManager::Finalize(void)
{
	// �`�惊�X�g�B
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
		SafeDelete(m_drawList[cnt]);
}

//=============================================================================
// �����N
//=============================================================================
void DrawListManager::Link(ObjectBase* object, int priority, Shader::PATTERN shaderPatternId)
{
	//----------------------------
	// �`�惊�X�g�Ƀ����N
	//----------------------------
	m_drawList[shaderPatternId]->Link(object, priority);
}

//=============================================================================
// �����N����
//=============================================================================
void DrawListManager::UnLink(ObjectBase* object, Shader::PATTERN shaderPatternId)
{
	//----------------------------
	// �`�惊�X�g�̃����N������
	//----------------------------
	m_drawList[shaderPatternId]->UnLink(object);
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`��
//=============================================================================
void DrawListManager::AllDraw(D3DXMATRIX vp)
{
	HRESULT				hr = S_OK;
	ObjectBase*			cur = nullptr;
	LPD3DXCONSTANTTABLE	vsc;
	LPD3DXCONSTANTTABLE	psc;

	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
	{
		if(cnt != Shader::PAT_2D)
			// 3D���_�ݒ�
			hr = m_device->SetVertexDeclaration(m_decl3D);
		else
			// 2D���_�ݒ�
			m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			//hr = m_device->SetVertexDeclaration(m_decl2D);

		if(FAILED(hr))
			MessageBox(NULL, "���_�ݒ莸�s", "SetVertexDeclaration", MB_OK);

		// �V�F�[�_�[�ݒ�
		m_shader->SetShader(&vsc, &psc, (Shader::PATTERN)cnt);

		/*if(cnt == Shader::PAT_2D)
		{
			// 2D�p�v���W�F�N�V����
			//D3DXMATRIX	proj2D(2/SCREEN_WIDTH,	0.0f,			 0.0f, 0.0f,
			//					0.0f,			2/SCREEN_HEIGHT, 0.0f, 0.0f,
			//					0.0f,			0.0f,			 1.0f, 0.0f,
			//					0.0f,			0.0f,			 0.0f, 1.0f);
			D3DXMATRIX	proj2D(1.0f,	0.0f, 0.0f, 0.0f,
								0.0f,	1.0f, 0.0f, 0.0f,
								0.0f,	0.0f, 1.0f, 0.0f,
								0.0f,	0.0f, 0.0f, 1.0f);
			vsc->SetMatrix(m_device, "gProj", &proj2D);
		}*/

		// �`��
		m_drawList[cnt]->AllDraw(vsc, psc, vp);
	}
}

// EOF
