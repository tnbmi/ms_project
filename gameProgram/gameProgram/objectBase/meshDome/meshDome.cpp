//*****************************************************************************
//
// ���b�V���̃h�[�� [meshDome.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshDome.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`�v�Z
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define VERTEX_NUM(x, z)		((x + 1) * (z + 1))					// �����_�v�Z
#define INDEX_NUM(x, z)		((x + 1) * (z * 2) + ((z - 1) * 2))	// ���C���f�b�N�X�v�Z
#define WORK_LINE(x, z, idx)	((idx / (INDEX_NUM(x, z) / z)) + 1)	// ��s�̃C���f�b�N�X�v�Z

//=============================================================================
// �R���X�g���N�^
//=============================================================================
MeshDome::MeshDome(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : ObjectBase(device, objectList, type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_vtx = nullptr;
	m_idx = nullptr;

	m_blockNum	= D3DXVECTOR2(0.0f, 0.0f);

	m_vtxNum	 = 0;
	m_indexNum	 = 0;
	m_polygonNum = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
MeshDome::~MeshDome(void)
{
	SafeDelete(m_vtx);
	SafeDelete(m_idx);
}

//=============================================================================
// ����
//=============================================================================
bool MeshDome::Create(MeshDome** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,
					D3DXVECTOR2 blockNum, float radius, float height,
					LPDIRECT3DTEXTURE9 texture,OBJECT_TYPE type)
{
	MeshDome* pointer = new MeshDome(device, objectList, type);
	if(!pointer->Initialize(blockNum, radius, height, texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool MeshDome::Initialize(D3DXVECTOR2 blockNum, float radius, float height, LPDIRECT3DTEXTURE9 texture)
{
	//----------------------------
	// �h�[���ɏo���邩�̃`�F�b�N
	//----------------------------
	if(blockNum.x < 3)
		return false;

	//----------------------------
	// �u���b�N�ݒ�
	//----------------------------
	// �u���b�N��
	m_blockNum.x = blockNum.x;
	m_blockNum.y = blockNum.y;

	// ���a
	m_scl.x = radius;
	m_scl.z = radius;

	// ����
	m_scl.y = height;

	//----------------------------
	// �h�[���ݒ�
	//----------------------------
	// �����_��
	m_vtxNum = (int)VERTEX_NUM(m_blockNum.x, m_blockNum.y);

	// ���_�̑��C���f�b�N�X��
	m_indexNum = (int)INDEX_NUM(m_blockNum.x, m_blockNum.y);

	// ���|���S����
	m_polygonNum = m_indexNum - 2;

	//----------------------------
	// ���_�ݒ�
	//----------------------------
	// ���_�o�b�t�@����
	m_vtx = new VERTEX[m_vtxNum];
	m_idx = new int[m_indexNum];

	// ���_���ݒ�
	SetVertex();

	// ���_�C���f�b�N�X�ݒ�
	SetVertexIndex();

	//----------------------------
	// �e�N�X�`���ݒ�
	//----------------------------
	m_texture = texture;

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void MeshDome::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void MeshDome::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void MeshDome::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
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
	m_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, m_vtxNum, m_polygonNum,
									m_idx, D3DFMT_INDEX32, m_vtx, sizeof(VERTEX));
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void MeshDome::SetVertex(void)
{
	int			workVtx;
	float		workAngleX = PAI * 2.0f / m_blockNum.x;
	float		workAngleY = PAI / (m_blockNum.y * 2.0f);
	D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / (float)m_blockNum.x,
									1.0f / (float)m_blockNum.y,
									0.0f);

	//----------------------------
	// �s�̏���
	//----------------------------
	for(float cntY = 0; cntY <= m_blockNum.y; ++cntY)
	{
		//----------------------------
		// ��̏���
		//----------------------------
		for(float cntX = 0; cntX <= m_blockNum.x; ++cntX)
		{
			// ���_�ԍ�
			workVtx = (int)((cntY * (m_blockNum.x + 1)) + cntX);

			// ���_���W
			m_vtx[workVtx].position.x = cos(workAngleX * cntX) * cos(workAngleY * cntY) * m_scl.x;
			m_vtx[workVtx].position.z = sin(workAngleX * cntX) * cos(workAngleY * cntY) * m_scl.z;
			m_vtx[workVtx].position.y = sin(workAngleY * cntY) * m_scl.y;

			// ���_�x�N�g��
			m_vtx[workVtx].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���ˌ�
			m_vtx[workVtx].color = m_color;

			// �e�N�X�`�����W
			m_vtx[workVtx].uv = D3DXVECTOR2(1.0f - texcord.x * (float)cntX, 1.0f - texcord.y * (float)cntY);
		}
	}
}

//=============================================================================
// ���_�C���f�b�N�X�ݒ�
//=============================================================================
void MeshDome::SetVertexIndex(void)
{	//----------------------------
	// �C���f�b�N�X�ݒ�
	//----------------------------
	for(int cnt = 0, cnt2 = 0; cnt < m_indexNum; ++cnt, ++cnt2)
	{
		// 0����Ȃ� ���� �d�����Ă���
		if( cnt != 0
		&& (((int)(cnt - VERTEX_NUM(m_blockNum.x, 1)) % (int)(VERTEX_NUM(m_blockNum.x, 1) + 2)) == 0
		 || ((int)(cnt - (VERTEX_NUM(m_blockNum.x, 1) + 2)) % (int)(VERTEX_NUM(m_blockNum.x, 1) + 2)) == 0))
		{
			// �d���̏���
			m_idx[cnt] = m_idx[cnt - 1];
			cnt2--;
		}
		else
		{
			// �d�����Ȃ�����
			m_idx[cnt] = (WORD)(((m_blockNum.x + 1) + (cnt2 / 2)) - ((cnt2 % 2) * (m_blockNum.x + 1)));
		}
	}
}

//=============================================================================
// ���_�T�C�Y�ϊ�
//=============================================================================
void MeshDome::SetVertexScl(void)
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
void MeshDome::SetVertexColor(void)
{
	// ���_�J���[
	m_vtx[0].color = m_color;
	m_vtx[1].color = m_color;
	m_vtx[2].color = m_color;
	m_vtx[3].color = m_color;
}

// EOF
