//*****************************************************************************
//
// 3D�|���S���̊�� [polygon3D.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "polygon3D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Polygon3D::Polygon3D(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : ObjectBase(device, objectList, type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_scl = D3DXVECTOR3(64.0f, 64.0f, 0.0f);

	m_norTexture = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Polygon3D::~Polygon3D(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Polygon3D::Create(Polygon3D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,
					LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type)
{
	Polygon3D* pointer = new Polygon3D(device, objectList, type);
	if(!pointer->Initialize(texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Polygon3D::Initialize(LPDIRECT3DTEXTURE9 texture)
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
void Polygon3D::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void Polygon3D::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void Polygon3D::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	//----------------------------
	// ���[���h�s��v�Z
	//----------------------------
	D3DXMATRIX wvp;
	D3DXMATRIX scaling, rotation, translate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_world);
	// ��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_world, &m_world, &rotation);
	// �ʒu�̔��f
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_world, &m_world, &translate);

	// ���[���h�r���[�v���W�F�N�V�����}�g���b�N�X
	wvp = m_world * vp;

	//----------------------------
	// �o�[�e�b�N�X
	//----------------------------
	if(vsc != nullptr)
	{
		// ���[���h�}�g���b�N�X�̐ݒ�
		vsc->SetMatrix(m_device, "gWorld", &m_world);

		// ���[���h�r���[�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
		vsc->SetMatrix(m_device, "gWVP", &wvp);
	}

	//----------------------------
	// �T���v���[����
	//----------------------------
	int texSampler = -1;
	int norSampler = -1;
	if(psc != nullptr)
	{
		// �T���v���[�C���f�b�N�X
		texSampler = psc->GetSamplerIndex("texSampler");

		// �T���v���[�X�e�[�g�p�����[�^
		m_device->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
		m_device->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
		m_device->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
		m_device->SetSamplerState(texSampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g��t�B���^���[�h��ݒ�
		m_device->SetSamplerState(texSampler, D3DSAMP_MAXANISOTROPY, 16);

		// �T���v���[�C���f�b�N�X
		norSampler = psc->GetSamplerIndex("norSampler");

		// �T���v���[�X�e�[�g�p�����[�^
		m_device->SetSamplerState(norSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
		m_device->SetSamplerState(norSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
		m_device->SetSamplerState(norSampler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
		m_device->SetSamplerState(norSampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(norSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g��t�B���^���[�h��ݒ�
		m_device->SetSamplerState(norSampler, D3DSAMP_MAXANISOTROPY, 16);
	}
	
	//----------------------------
	// �e�N�X�`���ݒ�
	//----------------------------
	// �e�N�X�`���̐ݒ�
	if(m_texture != nullptr)
	{
		if(texSampler >= 0)
			m_device->SetTexture(texSampler, m_texture);
		else
			m_device->SetTexture(0, m_texture);
	}
	
	// �@���e�N�X�`���̐ݒ�
	if(m_norTexture != nullptr)
		if(norSampler >= 0)
			m_device->SetTexture(norSampler, m_norTexture);

	//----------------------------
	// �`��
	//----------------------------
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vtx, sizeof(VERTEX));

	if(texSampler >= 0)
		m_device->SetTexture(texSampler, nullptr);
	else
		m_device->SetTexture(0, nullptr);

	if(norSampler >= 0)
		m_device->SetTexture(norSampler, nullptr);
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void Polygon3D::SetVertex(void)
{
	// ���W
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f * m_scl.x, 0.5f * m_scl.y, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x,  half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x, -half.y, half.z);

	// �e�N�X�`�����W
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�J���[
	m_vtx[0].color = m_color;
	m_vtx[1].color = m_color;
	m_vtx[2].color = m_color;
	m_vtx[3].color = m_color;

	// �@���x�N�g��
	for(int cnt = 0; cnt < 4; ++cnt)
	{
		m_vtx[cnt].normal =  D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXVec3Normalize(&m_vtx[cnt].normal, &m_vtx[cnt].normal);
	}
}

//=============================================================================
// ���_�T�C�Y�ϊ�
//=============================================================================
void Polygon3D::SetVertexScl(void)
{
	// ���W
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f * m_scl.x, 0.5f * m_scl.y, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x,  half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x, -half.y, half.z);
}

//=============================================================================
// ���_�F�ϊ�
//=============================================================================
void Polygon3D::SetVertexColor(void)
{
	// ���_�J���[
	m_vtx[0].color = m_color;
	m_vtx[1].color = m_color;
	m_vtx[2].color = m_color;
	m_vtx[3].color = m_color;
}

// EOF
