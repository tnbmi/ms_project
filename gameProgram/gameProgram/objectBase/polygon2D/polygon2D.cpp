//*****************************************************************************
//
// 2D�|���S���̊�� [polygon2D.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "polygon2D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Polygon2D::Polygon2D(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type) : ObjectBase(device, objectList, priority, type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_scl = D3DXVECTOR3(128.0f, 128.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Polygon2D::~Polygon2D(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Polygon2D::Create(Polygon2D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, int priority, OBJECT_TYPE type)
{
	Polygon2D* pointer = new Polygon2D(device, objectList, priority, type);
	if(!pointer->Initialize(texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Polygon2D::Initialize(LPDIRECT3DTEXTURE9 texture)
{
	//----------------------------
	// ���_���ݒ�
	//----------------------------
	SetVertex();

	//----------------------------
	// �e�N�X�`���ݒ�
	//----------------------------
	m_texture = texture;

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Polygon2D::Finalize(void)
{
	ObjectBase::~ObjectBase();
}

//=============================================================================
// �X�V
//=============================================================================
void Polygon2D::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void Polygon2D::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	//----------------------------
	// ���[���h�s��v�Z
	//----------------------------
	D3DXMATRIX wvp;
	D3DXMATRIX scaling, rotation, translate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_world);
	// �X�P�[���̔��f
/*	D3DXMatrixScaling(&scaling, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_world, &m_world, &scaling);
	// ��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_world, &m_world, &rotation);
	// �ʒu�̔��f
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_world, &m_world, &translate);

	// �s�񍇐�
	wvp = m_world * vp;*/

	//----------------------------
	// ���[���h�f�[�^�ݒ�
	//----------------------------
	// �o�[�e�b�N�X
	if(vsc != nullptr)
	{
		// ���[���h�}�g���b�N�X�̐ݒ�
		vsc->SetMatrix(m_device, "gWorld", &m_world);
		//vsc->SetMatrix(m_device, "gWVP", &wvp);
	}

	// �s�N�Z��
	/*if(psc != nullptr)
	{
		// ���[���h�}�g���b�N�X�̐ݒ�
		psc->SetMatrix(m_device, "gWorld", &m_world);
		psc->SetMatrix(m_device, "gWVP", &wvp);
	}*/

	//----------------------------
	// �T���v���[����
	//----------------------------
	unsigned int texSumpler = -1;
	if(psc != nullptr)
	{
		// �T���v���[�C���f�b�N�X
		texSumpler = psc->GetSamplerIndex("texSampler");

		// �T���v���[�X�e�[�g�p�����[�^
		m_device->SetSamplerState(texSumpler, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
		m_device->SetSamplerState(texSumpler, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
		m_device->SetSamplerState(texSumpler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k���t�B���^���[�h��ݒ�
		m_device->SetSamplerState(texSumpler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g��t�B���^���[�h��ݒ�
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAXANISOTROPY, 16);
}

	//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//----------------------------
	// �e�N�X�`���ݒ�
	//----------------------------
	if(texSumpler >= 0)
	{
		//�e�N�X�`���̐ݒ�
		m_device->SetTexture(texSumpler, m_texture);
	}
	else
	{
		m_device->SetTexture(0, m_texture);
	}

	//----------------------------
	// �`��
	//----------------------------
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
								2,
								m_vtx,
								sizeof(VERTEX));
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void Polygon2D::SetVertex(void)
{
	// ���W
	D3DXVECTOR3 half = D3DXVECTOR3(m_scl.x * 0.5f, m_scl.y * 0.5f, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x, -half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x,  half.y, half.z);

	//m_vtx[0].position = D3DXVECTOR3(0.0f,	0.0f,	half.z);
	//m_vtx[1].position = D3DXVECTOR3(half.x,	0.0f,	half.z);
	//m_vtx[2].position = D3DXVECTOR3(0.0f,	half.y,	half.z);
	//m_vtx[3].position = D3DXVECTOR3(half.x,	half.y,	half.z);

	// ���ˌ�
	m_vtx[0].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	m_vtx[1].color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	m_vtx[2].color = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	m_vtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);
}

// EOF
