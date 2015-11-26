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
bool Polygon3D::Create(Polygon3D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type)
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
	// �X�P�[���̔��f
	//D3DXMatrixScaling(&scaling, m_scl.x, m_scl.y, m_scl.z);
	//D3DXMatrixMultiply(&m_world, &m_world, &scaling);
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

		// �}�e���A���ݒ�
		D3DXVECTOR3 ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		vsc->SetVector(m_device, "gMatDiffuse", (D3DXVECTOR4*)&m_color);
		vsc->SetVector(m_device, "gMatAmbient", (D3DXVECTOR4*)&ambient);
	}

	//----------------------------
	// �T���v���[����
	//----------------------------
	unsigned int texSumpler = -1;
	if(psc != nullptr)
	{
		// �T���v���[�C���f�b�N�X
		texSumpler = psc->GetSamplerIndex("texSampler");

		// �T���v���[�X�e�[�g�p�����[�^
		m_device->SetSamplerState(texSumpler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
		m_device->SetSamplerState(texSumpler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g��t�B���^���[�h��ݒ�
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAXANISOTROPY, 16);
	}

	//----------------------------
	// �e�N�X�`���ݒ�
	//----------------------------
	if(texSumpler >= 0)
		//�e�N�X�`���̐ݒ�
		m_device->SetTexture(texSumpler, m_texture);
	else
		m_device->SetTexture(0, m_texture);

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

// EOF
