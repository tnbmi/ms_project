//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [roadpolygon.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "roadpolygon.h"
//=============================================================================
// �R���X�g���N�^
//=============================================================================
RoadPolygon::RoadPolygon(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_tex =NULL;
	m_vtxBuff = NULL;
	m_pos =D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rot = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	float m_fLength=0.0f;
	float m_fAngle=0.0f;
	m_diffuse = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
RoadPolygon::~RoadPolygon(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool RoadPolygon::Create(RoadPolygon** outPointer , LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize , POLYGON_CRITERIA crit )
{
	RoadPolygon* pointer = new RoadPolygon();
	if(!pointer->Initialize(device , texture , polygonSize , crit))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool RoadPolygon::Initialize( LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize ,POLYGON_CRITERIA crit )
{
	//----------------------------
	// �R�����g
	//----------------------------
	m_tex = texture;
	m_device = device;
	m_scl = D3DXVECTOR3( polygonSize.x , polygonSize.y , 0.0f );
	SetVertexPolygon( crit );
	m_crit = crit;
	return true;
}

//=============================================================================
// �I��
//=============================================================================
void RoadPolygon::Finalize(void)
{
	if( m_tex != NULL )
	{
		m_tex = NULL;
	}
	if( m_vtxBuff != NULL )
	{
		m_vtxBuff ->Release();
		m_vtxBuff =NULL;
	}
	delete this;
}

//=============================================================================
// �X�V
//=============================================================================
void RoadPolygon::Update(void)
{
	SetVertexPolygon( m_crit );
}

//=============================================================================
// �`��
//=============================================================================
void RoadPolygon::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc)
{
	//----------------------------
	// ���[���h�s��v�Z
	//----------------------------
	D3DXMATRIX wvp;
	D3DXMATRIX world, scaling, rotation, translate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&world);
	// �X�P�[���̔��f
	D3DXMatrixScaling(&scaling, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&world, &world, &scaling);
	// ��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&world, &world, &rotation);
	// �ʒu�̔��f
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&world, &world, &translate);

	//----------------------------
	// ���[���h�f�[�^�ݒ�
	//----------------------------
	// �o�[�e�b�N�X
	
	if(vsc != nullptr)
		// ���[���h�}�g���b�N�X�̐ݒ�
		vsc->SetMatrix(m_device, "gWorld", &world);

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

	//----------------------------
	// �e�N�X�`���ݒ�
	//----------------------------
	if(texSumpler >= 0)
		//�e�N�X�`���̐ݒ�
		m_device->SetTexture(texSumpler, m_tex);
	else
		m_device->SetTexture(0, m_tex);

	//----------------------------
	// �`��
	//----------------------------
	HRESULT hr;
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
								2,
								m_vtx,
								sizeof(VERTEX));
}
//=============================================================================
// �|���S���ݒ�
//=============================================================================
void RoadPolygon::SetVertexPolygon( POLYGON_CRITERIA crit )
{
	// ���W
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x, -half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x,  half.y, half.z);

	// ���_�J���[
	m_vtx[0].color = m_diffuse;
	m_vtx[1].color = m_diffuse;
	m_vtx[2].color = m_diffuse;
	m_vtx[3].color = m_diffuse;

	// �e�N�X�`�����W
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);
}

// EOF
