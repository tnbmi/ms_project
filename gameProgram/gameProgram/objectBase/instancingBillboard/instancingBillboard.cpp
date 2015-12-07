//*****************************************************************************
//
// �C���X�^���V���O�r���{�[�h�N���X [instancingBillboard.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "instancingBillboard.h"

//�\�[�g�p�֐�
int ZComp( const void *c1,const void *c2 )
{
	return (int)((*(InstancingBillboard::POLYGONDATA**)c2)->camRange - (*(InstancingBillboard::POLYGONDATA**)c1)->camRange);
}
//=============================================================================
// �R���X�g���N�^
//=============================================================================
InstancingBillboard::InstancingBillboard(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type ) :ObjectBase(device, objectList, type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	
	m_option.is_additiveSynthesis = false;
	m_option.is_zsort = false;
	m_option.is_zwrite = true;
	m_texture = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
InstancingBillboard::~InstancingBillboard(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool InstancingBillboard::Create(InstancingBillboard** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type )
{
	InstancingBillboard* pointer = new InstancingBillboard(device,objectList,priority,type);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}


bool InstancingBillboard::Create(InstancingBillboard** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type,
						const int polygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize )
{
	InstancingBillboard* pointer = new InstancingBillboard(device,objectList,priority,type);
	if(!pointer->Initialize( polygonMax,texFilePath,texSize,oneSize) )
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool InstancingBillboard::Initialize(void)
{
	//----------------------------
	// �R�����g
	//----------------------------

	CreateVtxBuffBill();
	LoadShaderBill();

	m_polygonMax = 5000;
	m_cursol = 0;

	//�C���X�^���V���O�p���z�񐶐�
	m_insArray = new POLYGONDATA*[ m_polygonMax ];

	for( int i = 0 ; i < m_polygonMax ; i++ )
	{
		m_insArray[ i ] = new POLYGONDATA;
		m_insArray[ i ]->isDelete = true;
		m_insArray[ i ]->camRange = 0;
	}

	//�C���X�^���V���O�p�o�b�t�@����
	CreateInstancingBuff();
	
	//�C���f�b�N�X�o�b�t�@�𐶐��i�C���X�^���V���O��DrawIndexedPrimitive����Ȃ��ƃ_��������)
	CreateIdxBuff();

	return true;
}


bool InstancingBillboard::Initialize( const int polygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize )
{
	//----------------------------
	// �R�����g
	//----------------------------

	CreateVtxBuffBill();
	LoadShaderBill();

	m_polygonMax = polygonMax;
	m_cursol = 0;

	//�C���X�^���V���O�p���z�񐶐�
	m_insArray = new POLYGONDATA*[ m_polygonMax ];

	for( int i = 0 ; i < m_polygonMax ; i++ )
	{
		m_insArray[ i ] = new POLYGONDATA;
		m_insArray[ i ]->isDelete = true;
		m_insArray[ i ]->camRange = 0;
	}

	//�C���X�^���V���O�p�o�b�t�@����
	CreateInstancingBuff();
	
	//�C���f�b�N�X�o�b�t�@�𐶐��i�C���X�^���V���O��DrawIndexedPrimitive����Ȃ��ƃ_��������)
	CreateIdxBuff();

	//�e�N�X�`�����[�h
	LoadTexture( texFilePath );
	SetVtxBuffTexInfo( texSize,oneSize );
	

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void InstancingBillboard::Finalize(void)
{

	if( m_instancingBuff != nullptr )
	{
		m_instancingBuff->Release();
		m_instancingBuff = nullptr;
	}

	if( m_vtxBuff != nullptr )
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}

	if( m_idxBuff != nullptr )
	{
		m_idxBuff->Release();
		m_idxBuff = nullptr;
	}

	if( m_decl != nullptr )
	{
		m_decl->Release();
		m_decl = nullptr;
	}

	if( m_texture != nullptr )
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	if( m_insArray != nullptr )
	{
		for( int i = 0 ; i < m_polygonMax ; i++ )
		{
			delete  m_insArray[ i ] ;
		}

		delete []m_insArray;

		m_insArray = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void InstancingBillboard::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void InstancingBillboard::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	//�C���X�^���V���O�p�o�b�t�@�ɏ�������
	//�A�b�v�f�[�g���ǂ̏��ԂŌĂ΂�邩�킩��Ȃ��̂ł����ł�邵���Ȃ�
	ApplyInstancingBuff();

	if( m_usingPolygonSum != 0)
	{
		ApplyOption();

		//�X�g���[���ւ̃f�[�^������H
		m_device->SetStreamSourceFreq( 0,D3DSTREAMSOURCE_INDEXEDDATA | m_usingPolygonSum );
		m_device->SetStreamSourceFreq( 1,D3DSTREAMSOURCE_INSTANCEDATA | 1 );
		m_device->SetVertexDeclaration( m_decl );

		//�X�g���[���ɗ���
		m_device->SetStreamSource( 0,m_vtxBuff,0,m_vtxSize );
		m_device->SetStreamSource( 1,m_instancingBuff,0,sizeof( INSTANCINGDATA ) );

		
		//�e�N�X�`���̃Z�b�g
		m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ),D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ), D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
		m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ), D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
		m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ), D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	
		m_device->SetTexture( psc->GetSamplerIndex( "samp" ),m_texture );

		D3DXMATRIX mtx_view,mtx_proj;
		m_device->GetTransform( D3DTS_VIEW,&mtx_view );
		m_device->GetTransform( D3DTS_PROJECTION,&mtx_proj );

		vsc->SetMatrix( m_device,"mtx_vp",&vp );

		//offset�œ�����
		D3DXVECTOR2 uv_size;
		uv_size.x = m_oneSize.x / m_texSize.x;
		uv_size.y = m_oneSize.y / m_texSize.y;

		vsc->SetFloatArray( m_device,"uv_size",uv_size,2 );


		m_device->SetIndices( m_idxBuff );

		//�A���t�@�e�X�g
		m_device->SetRenderState( D3DRS_ALPHATESTENABLE,true );
		m_device->SetRenderState( D3DRS_ALPHAREF, 0 );
		m_device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );

		//�`��
		m_device->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,0,0,4,0,2 );

		//�A���t�@�e�X�g����
		m_device->SetRenderState( D3DRS_ALPHATESTENABLE,false );

		//�㏈��
		m_device->SetStreamSourceFreq( 0,1 );
		m_device->SetStreamSourceFreq( 1,1 );

	}

	//option��߂�

	//���Z��������߂�
	m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA );
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//Z�o�b�t�@�ւ̏������݂�����
	m_device->SetRenderState( D3DRS_ZWRITEENABLE,true );
}

//--------------------------------------------------------------------------
//Apply
//--------------------------------------------------------------------------
void InstancingBillboard::ApplyInstancingBuff()
{
	D3DXMATRIX mtx_ident;
	D3DXMATRIX mtx_view_inv;

	//�r���{�[�h���[�h�̎��̂݃r���[�̋t�s����쐬
	D3DXMATRIX mtx_view;
	m_device->GetTransform( D3DTS_VIEW,&mtx_view );
	//mtx_view = m_viewMtx;//�������ꂽ��g��
	D3DXMatrixInverse( &mtx_view_inv,NULL,&mtx_view );

	//�\�[�g�I�v�V����
	if( m_option.is_zsort )
	{
		//�f�[�^�Z�b�g
		for( int i = 0 ; i < m_polygonMax; i++ )
		{
			if( !m_insArray[i]->isDelete )
			{
				D3DXVECTOR3 r = m_cameraPos - m_insArray[i]->pos;
				m_insArray[i]->camRange = D3DXVec3Length(&r);
			}
			else
			{
				m_insArray[i]->camRange = 0;
			}
		}

		//�\�[�g
		qsort( m_insArray,m_polygonMax,sizeof( POLYGONDATA* ),ZComp );

	}
	

	//������
	D3DXMatrixIdentity( &mtx_ident );

	INSTANCINGDATA *ins;

	m_usingPolygonSum = 0;

	m_instancingBuff->Lock( 0,0,(void**)&ins,0 );

	for( int i = 0 ; i < m_polygonMax ; i++ )
	{
		if( !m_insArray[i]->isDelete )
		{
			D3DXVECTOR3 pos,scl,rot;

			pos = m_insArray[i]->pos;
			scl = m_insArray[i]->scl;
			rot = m_insArray[i]->rot;

			D3DXMATRIX mtx_world,mtx_scl,mtx_rot,mtx_trans;

			//������
			mtx_world = mtx_ident;

			D3DXMatrixScaling( &mtx_scl,scl.x,scl.y,scl.z );
			D3DXMatrixRotationYawPitchRoll( &mtx_rot,rot.y,rot.x,rot.z );
			D3DXMatrixTranslation( &mtx_trans,pos.x,pos.y,pos.z );


				D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_scl );
				D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_view_inv );

				mtx_world._41=0.0f;
				mtx_world._42=0.0f;
				mtx_world._43=0.0f;

				D3DXMatrixMultiply( &mtx_world,&mtx_world,&mtx_trans );
		

			ins[ m_usingPolygonSum ].mtx = mtx_world;
			//ins[ m_using_polygon_sum ].col = (*it)->col;
			ins[ m_usingPolygonSum ].col.x = m_insArray[i]->col.r;
			ins[ m_usingPolygonSum ].col.y = m_insArray[i]->col.g;
			ins[ m_usingPolygonSum ].col.z = m_insArray[i]->col.b;
			ins[ m_usingPolygonSum ].col.w = m_insArray[i]->col.a;
			ins[ m_usingPolygonSum ].uv_offset = m_insArray[i]->uvOffset;

			m_usingPolygonSum++;
		}

	}

	m_instancingBuff->Unlock();
}

//---------------------------------------------------------------------------
//CreateVtx
//---------------------------------------------------------------------------

bool InstancingBillboard::CreateVtxBuffBill()
{
	m_vtxSize = sizeof( VTXBILL );

	//��{�}�`�쐬
	if( FAILED( m_device->CreateVertexBuffer(m_vtxSize*4,0,0,D3DPOOL_MANAGED,&m_vtxBuff,NULL ) ) )
	{
		MessageBox( NULL,"���_�������s","�G���[",MB_OK );
		return false;
	}

	VTXBILL *vtx;

	//���b�N
	m_vtxBuff->Lock( 0,0,(void**)&vtx,0 );

	vtx[0].pos = D3DXVECTOR3( -0.5,0.5,0 );
	vtx[0].uv  = D3DXVECTOR2( 0,0 );

	vtx[1].pos = D3DXVECTOR3( 0.5,0.5,0 );
	vtx[1].uv  = D3DXVECTOR2( m_oneSize.x/m_texSize.x,0 );

	vtx[2].pos = D3DXVECTOR3( -0.5,-0.5,0 );
	vtx[2].uv  = D3DXVECTOR2( 0,m_oneSize.y/m_texSize.y );

	vtx[3].pos = D3DXVECTOR3( 0.5,-0.5,0 );
	vtx[3].uv  = D3DXVECTOR2( m_oneSize.x/m_texSize.x,m_oneSize.y/m_texSize.y );

	m_vtxBuff->Unlock();

	return true;
}

//--------------------------------------------------------------------------
//LoadShader
//--------------------------------------------------------------------------

void InstancingBillboard::LoadShaderBill()
{
	//�V�F�[�_�[����
	LPD3DXBUFFER code,err;

	code = NULL;
	err = NULL;

	//�f�[�^�\����`
	//���_�v�f�z������
	D3DVERTEXELEMENT9 elem[] ={
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION, 0},//�ʒu
		{0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},//�t��
		{1,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},//�ʍs��
		{1,16,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},
		{1,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3},
		{1,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4},
		{1,64,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,5},//UvOffset
		{1,72,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},//�J���[
		D3DDECL_END()
	};

	//�쐬
	if( FAILED(m_device->CreateVertexDeclaration( elem,&m_decl ) ) )
	{
		MessageBox(NULL,"�f�[�^�\����`�������s","�G���[",MB_OK );
	}
}

//---------------------------------------------------------------------------------
//CreateInstancingBuff
//---------------------------------------------------------------------------------

bool InstancingBillboard::CreateInstancingBuff()
{

	//�C���X�^���V���O�p�o�b�t�@�쐬
	if( FAILED( m_device->CreateVertexBuffer( sizeof(INSTANCINGDATA) * m_polygonMax, 0,0,D3DPOOL_MANAGED,&m_instancingBuff,NULL ) ) )
	{
		MessageBox(NULL,"�C���X�^���V���O�p�o�b�t�@�������s","�G���[",MB_OK );
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------------
//CreateIdxBuff
//---------------------------------------------------------------------------------

bool InstancingBillboard::CreateIdxBuff()
{
	//index�o�b�t�@
	if( FAILED( m_device->CreateIndexBuffer( sizeof(WORD)*4,
										 D3DUSAGE_WRITEONLY,
										 D3DFMT_INDEX16,
										 D3DPOOL_MANAGED,
										 &m_idxBuff,
										 NULL) ) )
	{
		return false;
	}

	WORD *idx;

	m_idxBuff->Lock( 0,0,(void**)&idx,0 );

	idx[0] = 0;
	idx[1] = 1;
	idx[2] = 2;
	idx[3] = 3;

	m_idxBuff->Unlock();

	return true;
}

//
//
//

InstancingBillboard::POLYGONDATA *InstancingBillboard::LentInsPolygonData()
{
	//�݂��o����Idx��ۑ����A�����R�X�g���y������
	for( int i = 0 ; i < m_polygonMax ; i++ )
	{
		if( m_insArray[m_cursol]->isDelete )
		{
			m_insArray[m_cursol]->isDelete = false;
			return m_insArray[m_cursol];
		}

		m_cursol++;

		if( m_cursol >= m_polygonMax )
		{
			m_cursol = 0;
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------------------
//ApplyOption
//-------------------------------------------------------------------------------------

void InstancingBillboard::ApplyOption()
{
	
	//���Z��������`�惉�C���H
	if( this->m_option.is_additiveSynthesis )
	{
		m_device->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
		m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA );
		m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE );
	}

	if( !this->m_option.is_zwrite )
	{
		m_device->SetRenderState( D3DRS_ZWRITEENABLE,false );
	}

}

//-----------------------------------------------------------------------------------
//LoadTexture
//-----------------------------------------------------------------------------------

void InstancingBillboard::LoadTexture( const char *FilePath )
{
	if( m_texture != NULL )
	{
		m_texture->Release();
		m_texture = NULL;
		//�e�N�X�`�����[�h
		D3DXCreateTextureFromFile(m_device,FilePath,&m_texture);
	}
	else
	{
		D3DXCreateTextureFromFile(m_device,FilePath,&m_texture);
	}
}

void InstancingBillboard::SetVtxBuffTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 oneSize )
{
	VTXBILL *vtx;

	m_texSize = texSize;
	m_oneSize = oneSize;

	//���b�N
	m_vtxBuff->Lock( 0,0,(void**)&vtx,0 );

	vtx[0].pos = D3DXVECTOR3( -0.5,0.5,0 );
	vtx[0].uv  = D3DXVECTOR2( 0,0 );

	vtx[1].pos = D3DXVECTOR3( 0.5,0.5,0 );
	vtx[1].uv  = D3DXVECTOR2( m_oneSize.x/m_texSize.x,0 );

	vtx[2].pos = D3DXVECTOR3( -0.5,-0.5,0 );
	vtx[2].uv  = D3DXVECTOR2( 0,m_oneSize.y/m_texSize.y );

	vtx[3].pos = D3DXVECTOR3( 0.5,-0.5,0 );
	vtx[3].uv  = D3DXVECTOR2( m_oneSize.x/m_texSize.x,m_oneSize.y/m_texSize.y );

	m_vtxBuff->Unlock();
}

// EOF