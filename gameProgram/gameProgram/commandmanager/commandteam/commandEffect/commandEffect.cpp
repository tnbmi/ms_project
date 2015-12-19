//*****************************************************************************
//
// �R�}���h�G�t�F�N�g�p�|���S�� [commandEffect.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandEffect.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CommandEffect::CommandEffect(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : Polygon2D(device, objectList, type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CommandEffect::~CommandEffect(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool CommandEffect::Create(CommandEffect** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type)
{
	CommandEffect* pointer = new CommandEffect(device, objectList, type);
	if(!pointer->Initialize(texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool CommandEffect::Initialize(LPDIRECT3DTEXTURE9 texture)
{
	//----------------------------
	// �e�̏������Ăяo��
	//----------------------------
	Polygon2D::Initialize(texture);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void CommandEffect::Finalize(void)
{
	//----------------------------
	// �e�̏I���Ăяo��
	//----------------------------
	Polygon2D::Finalize();
}

//=============================================================================
// �X�V
//=============================================================================
void CommandEffect::Update(void)
{
	//----------------------------
	// �e�̍X�V�Ăяo��
	//----------------------------
	Polygon2D::Update();
}

//=============================================================================
// �`��
//=============================================================================
void CommandEffect::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	// ���Z����
	m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//----------------------------
	// �e�̕`��Ăяo��
	//----------------------------
	Polygon2D::Draw(vsc, psc, vp);

	// ���Z������������Ȃ�
	m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

// EOF
