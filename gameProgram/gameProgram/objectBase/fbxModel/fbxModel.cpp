//*****************************************************************************
//
// FbxModel�N���X [FbxModel.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "fbxModel.h"


//=============================================================================
// �R���X�g���N�^
//=============================================================================
FbxModel::FbxModel(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type) : ObjectBase(device, objectList, type)
{
	//----------------------------
	// �����o�[������
	//----------------------------

	m_pos = D3DXVECTOR3(0,0,0);
	m_rot = D3DXVECTOR3(0,0,0);
	m_scl = D3DXVECTOR3(-1,1,1);

	//�A�j���[�V�������[�v
	m_isAnimRoop = false;
	m_isBlendRoop = false;


	m_startKeyFrame = 0;
	m_endKeyFrame = 0;
	m_curKeyFrame = 0;
	m_animTime = 0;
	m_blendTime = 0;

	m_blendWeight = _blendFrame;
	m_blendStartKeyFrame = 0;
	m_blendEndKeyFrame   = 0;
	m_blendCurKeyFrame   = 0;

	




}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
FbxModel::~FbxModel(void)
{

}

//=============================================================================
// ����
//=============================================================================
bool FbxModel::Create(FbxModel** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority , OBJECT_TYPE type,char *LoadModelPath)
{
	FbxModel* pointer = new FbxModel( device,objectList,priority,type );
	if(!pointer->Initialize())
		return false;

	pointer->LoadFbxModel( LoadModelPath );
	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool FbxModel::Initialize(void)
{
	//----------------------------
	// �R�����g
	//----------------------------
	//�V�F�[�_�[���Ȃ���̔ėp���Ƃ��͖���
	//���_�v�f�z������
	D3DVERTEXELEMENT9 elem[] ={
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION, 0},
		{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,  0},
		{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
		{0,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},
		{0,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},
		D3DDECL_END()
	};
	//�f�J�[���쐬
	m_device->CreateVertexDeclaration(elem,&m_decl);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void FbxModel::Finalize(void)
{
	if( m_part != nullptr )
	{

		for( int i = 0 ; i < m_partSum ; i++ )
		{
			for( int j = 0 ; j < m_part[i].dataSum ; j++ )
			{
				if( m_part[i].dataArray[j].idxSum > 0 )
				{
					m_part[i].dataArray[j].idxBuff->Release();
					m_part[i].dataArray[j].vtxBuff->Release();

					m_part[i].dataArray[j].tex->Release();

					delete []m_part[i].dataArray[j].texName;

					delete []m_part[i].dataArray[j].usingBoneArray;

				}
			}
			delete []m_part[i].dataArray;
		}
	
		//�����͐�΂Ȃ��Ƃ�������
		delete []m_part;

		m_part = nullptr;

	}

	if( m_boneArray != nullptr )
	{
		for( int i = 0 ; i < m_boneSum ; i++ )
		{
			if( m_boneArray[i].keyMax != 0 )
			{
				delete []m_boneArray[i].keyFrameArray;
			}


			if( m_boneArray[i].childNum != 0 )
			{
				delete []m_boneArray[i].childIdxArray;
			}
		}

		//���Ȃ��N��������Ȃ�
		if( m_boneSum != 0 )
		{
			delete []m_boneArray;
			m_boneArray = nullptr;
		}

	}
}

//=============================================================================
// �X�V
//=============================================================================
void FbxModel::Update(void)
{
	UpdateAnimation();
}

//=============================================================================
// �`��
//=============================================================================
void FbxModel::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	D3DXMATRIX mtxWorld,mtxScl,mtxTrans,mtxRot;

	if( m_scl.x <= -1 )
	{
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixScaling(&mtxScl,m_scl.x,m_scl.y,m_scl.z);
	D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxScl);
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_rot.y,m_rot.x,m_rot.z);
	D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRot);
	D3DXMatrixTranslation(&mtxTrans,m_pos.x,m_pos.y,m_pos.z);
	D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxTrans);

	if( m_boneSum != 0 )
	{
		RecursiveBone( &m_boneArray[0],mtxWorld );
	}

	m_device->SetVertexDeclaration( m_decl );

	vsc->SetMatrix( m_device,"mtx_vp",&vp );
	vsc->SetInt( m_device,"no_bone",m_noBone);
	vsc->SetMatrix( m_device,"mtx_world",&mtxWorld );
	vsc->SetFloatArray( m_device,"light_vec",D3DXVECTOR3(0.5,-0.5,0 ),3 );

	for( int i = 0 ; i < m_partSum ; i++ )
	{
		for( int j = 0 ; j < m_part[i].dataSum ; j++ )
		{
			
				D3DXMATRIX bone_mtx[50];

				SetUpUsingBoneArray( bone_mtx,i,j );

				vsc->SetMatrixArray( m_device,"mtx_bone",bone_mtx,50 );
				vsc->SetFloatArray(  m_device,"mat_diffuse",m_part[i].dataArray[j].mat.diffuse,4 );
				m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ),D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
				m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ), D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
				m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ), D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
				m_device->SetSamplerState( psc->GetSamplerIndex( "samp" ), D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

				m_device->SetTexture( psc->GetSamplerIndex( "samp" ),m_part[i].dataArray[j].tex );

				m_device->SetStreamSource( 0,m_part[i].dataArray[j].vtxBuff,0,sizeof(FBXVTX) );
				m_device->SetIndices( m_part[i].dataArray[j].idxBuff );
				m_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
					0,0,m_part[i].dataArray[j].vtxSum,0,m_part[i].dataArray[j].idxSum/3 );
		
		}
	}

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//=============================================================================
//�A�j���[�V�����J�n
//=============================================================================
void FbxModel::StartAnimation( const int startKeyFrame,const int endKeyFrame,const bool isRoop )
{
	//�O��̃A�j���[�V���������u�����h�Ƃ��Ďg��
	//�u�����h���
	m_blendStartKeyFrame = m_startKeyFrame;
	m_blendEndKeyFrame   = m_endKeyFrame;
	m_blendCurKeyFrame   = m_curKeyFrame;
	m_blendTime          = m_animTime;
	m_isBlendRoop        = m_isAnimRoop;

	//�A�j���[�V����
	m_startKeyFrame = startKeyFrame;
	m_curKeyFrame   = m_startKeyFrame;
	m_endKeyFrame   = endKeyFrame;
	m_isAnimRoop = isRoop;
	m_animTime = 0;

	m_blendWeight = 0;
}

//=============================================================================
//�A�j���[�V�����X�V
//=============================================================================

void FbxModel::UpdateAnimation()
{
	//���Ԃ͓����ł���
	//���[�g�{�[���̎��Ԃ��\���Ďg����
	float time = float( ( m_animTime - 0 ) ) / float( ( 1 - 0 ) );// ���Ԃ�}��ϐ���
	float btime= float( ( m_blendTime- 0 ) ) / float( ( 1 - 0 ) );
	float blendWeight = float( ( m_blendWeight ) ) / float( ( _blendFrame ) );

	float rate = Cube( blendWeight );

	for( int i = 0 ; i < m_boneSum ; i++ )
	{
		if( m_boneArray[i].keyMax > 0 )
		{
			
			//������
			D3DXMATRIX r,s,t;
			D3DXMatrixIdentity( &s );
			D3DXMatrixIdentity( &r );
			D3DXMatrixIdentity( &t );

			//���[�V�����v�Z
			D3DXVECTOR3 scl,trans;
			D3DXQUATERNION q ;
			
			scl.x = time * m_boneArray[i].keyFrameArray[m_curKeyFrame+1].scl.x + (1.0f - time ) *m_boneArray[i].keyFrameArray[m_curKeyFrame].scl.x;
			scl.y = time * m_boneArray[i].keyFrameArray[m_curKeyFrame+1].scl.y + (1.0f - time ) *m_boneArray[i].keyFrameArray[m_curKeyFrame].scl.y;
			scl.z = time * m_boneArray[i].keyFrameArray[m_curKeyFrame+1].scl.z + (1.0f - time ) *m_boneArray[i].keyFrameArray[m_curKeyFrame].scl.z;
														
			trans.x = time * m_boneArray[i].keyFrameArray[m_curKeyFrame+1].trans.x + (1.0f - time ) *m_boneArray[i].keyFrameArray[m_curKeyFrame].trans.x;
			trans.y = time * m_boneArray[i].keyFrameArray[m_curKeyFrame+1].trans.y + (1.0f - time ) *m_boneArray[i].keyFrameArray[m_curKeyFrame].trans.y;
			trans.z = time * m_boneArray[i].keyFrameArray[m_curKeyFrame+1].trans.z + (1.0f - time ) *m_boneArray[i].keyFrameArray[m_curKeyFrame].trans.z;

			D3DXQuaternionSlerp( &q,&m_boneArray[i].keyFrameArray[m_curKeyFrame].rot,&m_boneArray[i].keyFrameArray[m_curKeyFrame+1].rot,time );

			//���[�V�����u�����h�v�Z
			D3DXVECTOR3 bscl,btrans;
			D3DXQUATERNION bq;

			bscl.x = btime * m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].scl.x + (1.0f - btime ) *m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].scl.x;
			bscl.y = btime * m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].scl.y + (1.0f - btime ) *m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].scl.y;
			bscl.z = btime * m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].scl.z + (1.0f - btime ) *m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].scl.z;
														
			btrans.x = btime * m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].trans.x + (1.0f - btime ) *m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].trans.x;
			btrans.y = btime * m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].trans.y + (1.0f - btime ) *m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].trans.y;
			btrans.z = btime * m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].trans.z + (1.0f - btime ) *m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].trans.z;

			D3DXQuaternionSlerp( &bq,&m_boneArray[i].keyFrameArray[m_blendCurKeyFrame].rot,&m_boneArray[i].keyFrameArray[m_blendCurKeyFrame+1].rot,btime ); 

			//�u�����h���[�g����



			s._11 = scl.x * rate + bscl.x * (1.0f-rate);
			s._22 = scl.y * rate + bscl.y * (1.0f-rate);
			s._33 = scl.z * rate + bscl.z * (1.0f-rate);

			t._41 = trans.x * rate + btrans.x * (1.0f - rate );
			t._42 = trans.y * rate + btrans.y * (1.0f - rate );
			t._43 = trans.z * rate + btrans.z * (1.0f - rate );

			D3DXQuaternionSlerp( &q,&bq,&q,rate );

			D3DXMatrixRotationQuaternion( &r,&q );

			D3DXMATRIX test;
			D3DXMatrixIdentity( &test );

			D3DXMatrixMultiply( &test,&test,&s );
			D3DXMatrixMultiply( &test,&test,&r );
			D3DXMatrixMultiply( &test,&test,&t );

			m_boneArray[ i ].mtxAnim = test;

			
		}
	}

		//�u�����h��d
		m_blendWeight++;
		m_blendTime++;
		m_animTime++;

		if( rate >= 1 )
		{
			blendWeight = 1;
			m_blendWeight = _blendFrame;
			rate = 1;
		}

		if( btime >=1 )
		{
			btime = 1;
			m_blendTime = 1;

			if( m_blendCurKeyFrame < m_blendEndKeyFrame-1 )
			{
				m_blendCurKeyFrame++;
				btime = 0;
				m_blendTime = 0;
			}
			else if( m_isBlendRoop )
			{
				time = 0;
				m_blendTime = 0;
				m_blendCurKeyFrame = m_blendStartKeyFrame;
			}
		}

			
		if( time >= 1 )
		{
			time = 1;
			m_animTime =  1;

			if( m_curKeyFrame < m_endKeyFrame-1 )
			{
				
				m_curKeyFrame++;
				

				time = 0;
				m_animTime = 0;
			}
			else if( m_isAnimRoop )
			{
				time = 0;
				m_animTime = 0;
				m_curKeyFrame = m_startKeyFrame;
			}
		}
}

//=============================================================================
//�{�[���s��Z�b�g
//=============================================================================

void FbxModel::SetUpUsingBoneArray( D3DXMATRIX *mtxArray,const int idx1,const int idx2 )
{
	//�ꉞ������
	for( int i = 0 ; i < _usingBoneMax ; i++ )
	{
		D3DXMatrixIdentity( &mtxArray[i] );
	}

	//�Z�b�g
	for( int i = 0 ; i < m_part[idx1].dataArray[idx2].usingBoneSum ; i++ )
	{
		int usingIndex = m_part[idx1].dataArray[idx2].usingBoneArray[ i ];
		mtxArray[i] = m_boneArray[ usingIndex ].mtxWorld;
	}
}

//=============================================================================
//�{�[���s��X�V �ċA
//=============================================================================

void FbxModel::RecursiveBone( BONEDATA *bone,const D3DXMATRIX &mtx )
{
	D3DXMatrixMultiply( &bone->mtxTemp,&bone->mtxAnim,&mtx );
	D3DXMatrixMultiply( &bone->mtxWorld,&bone->offsetBoneMtx,&bone->mtxTemp );

	//�q�ɍs���n��
	for( int i = 0 ; i < bone->childNum ; i++ )
	{
		int idx = bone->childIdxArray[i];
		RecursiveBone( &m_boneArray[idx],bone->mtxTemp );
	}
}

//=============================================================================
//Fbx���f�����[�h
//=============================================================================

bool FbxModel::LoadFbxModel( const char *loadModelPath )
{
	LPDIRECT3DDEVICE9 device = m_device;
	FILE *file;
	fopen_s( &file,loadModelPath,"rb" );

	//�p�[�c���ǂݍ���
	fread( &m_partSum,sizeof( int ),1,file );

	m_part = new FBXPARTDATA[ m_partSum ];

	for( int i = 0 ; i < m_partSum ; i++ )
	{
		//�f�[�^����ǂݍ���
		int data;
		fread( &data,sizeof(int),1,file );
		m_part[i].dataSum = data;

		m_part[i].dataArray = new FBXMODELDATA[ data ];

		for( int j = 0 ; j < data ; j++ )
		{
				int len;

				//�������ǂݍ���
				fread( &len,sizeof( int ),1,file );
				//�t�@�C���p�X��������
				//�����ꍇ��NOTEX�ƂȂ�
				m_part[ i ].dataArray[j].texName = new char [ len ];
				fread( m_part[i].dataArray[j].texName,sizeof(char),len,file );

				char whiteTexPath[128] = "../resources/texture/test_0.png";
				char texPath[128] = "../resources/texture/";

				if( strcmp( m_part[i].dataArray[j].texName,"NOTEX" ) == 0 )
				{
					D3DXCreateTextureFromFile(device,whiteTexPath,&m_part[i].dataArray[j].tex);
				}
				else
				{
					//�A����̕�����
					int tlen = strlen( texPath ) + len;

					//�o�b�t�@����
					char *c = new char[len];
					strcpy_s( c,len,m_part[ i ].dataArray[j].texName);

					//��U�폜
					delete []m_part[ i ].dataArray[j].texName;

					//����
					m_part[ i ].dataArray[j].texName = new char[ tlen ];
					//�p�X�A��
					sprintf_s( m_part[ i ].dataArray[j].texName,tlen,"%s%s",texPath,c );

					//�o�b�t�@�폜
					delete []c;

					//�e�N�X�`�����[�h
					//D3DXCreateTextureFromFile(device,whiteTexPath,&m_part[i].dataArray[j].tex);
					D3DXCreateTextureFromFile(device,m_part[i].dataArray[j].texName,&m_part[i].dataArray[j].tex);
				}


				//�}�e���A���ǂݍ���
				fread( &m_part[i].dataArray[j].mat.ambent.x,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.ambent.y,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.ambent.z,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.ambent.w,sizeof( float ),1,file );

				fread( &m_part[i].dataArray[j].mat.diffuse.x,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.diffuse.y,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.diffuse.z,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.diffuse.w,sizeof( float ),1,file );

				fread( &m_part[i].dataArray[j].mat.emission.x,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.emission.y,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.emission.z,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.emission.w,sizeof( float ),1,file );

				fread( &m_part[i].dataArray[j].mat.specular.x,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.specular.y,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.specular.z,sizeof( float ),1,file );
				fread( &m_part[i].dataArray[j].mat.specular.w,sizeof( float ),1,file );

				fread( &m_part[i].dataArray[j].mat.shiniess,sizeof( float ),1,file );

				//���_��������
				fread( &m_part[i].dataArray[j].vtxSum,sizeof( int ),1,file );

				//�{�[�����̏�������
				int bone_sum = 0;
				fread( &bone_sum,sizeof(int),1,file );

				if( bone_sum == 0 )
				{
					m_noBone = 1;
				}
				else
				{
					m_noBone = 0;
				}

				//���_�쐬
				HRESULT hr = device->CreateVertexBuffer(sizeof(FBXVTX)*m_part[i].dataArray[j].vtxSum,D3DUSAGE_WRITEONLY,0,D3DPOOL_MANAGED,&m_part[i].dataArray[j].vtxBuff,NULL);
				FBXVTX *vtx;
				m_part[i].dataArray[j].vtxBuff->Lock(0,0,(void**)&vtx,0);//���b�N���Ă���l�ύX
				for( int v = 0 ; v < m_part[i].dataArray[j].vtxSum ; v++ )
				{
					double pos[3],nor[3],uv[2];
					fread( &pos[0],sizeof(double),1,file );
					fread( &pos[1],sizeof(double),1,file );
					fread( &pos[2],sizeof(double),1,file );

					fread( &nor[0],sizeof(double),1,file );
					fread( &nor[1],sizeof(double),1,file );
					fread( &nor[2],sizeof(double),1,file );

					fread( &uv[0],sizeof(double),1,file );
					fread( &uv[1],sizeof(double),1,file );

					double weight[4],boneidx[4];

					//�{�[��������΂ȁI
					if( bone_sum != 0 )
					{

						for( int inf = 0 ; inf < 4 ; inf++ )
						{
							fread( &weight[inf],sizeof( double ) ,1,file );
							fread( &boneidx[inf],sizeof( double ),1,file );
						}
					}
					else
					{
						weight[0] = 0;
						weight[1] = 0;
						weight[2] = 0;
						weight[3] = 0;

						boneidx[0] = -1;
						boneidx[1] = -1;
						boneidx[2] = -1;
						boneidx[3] = -1;
					}

				

					//�o�b�t�@�ɏ�������
					//test
					vtx[v].vtx = D3DXVECTOR3( (float)pos[0],(float)pos[1],(float)pos[2] );
					vtx[v].nor = D3DXVECTOR3( (float)nor[0],(float)nor[1],(float)nor[2] );
					vtx[v].uv  = D3DXVECTOR2( (float)uv[0],(float)uv[1] );
					vtx[v].weight = D3DXVECTOR4( (float)weight[0],(float)weight[1],(float)weight[2],(float)weight[3] );
					vtx[v].boneIdx= D3DXVECTOR4( (float)boneidx[0],(float)boneidx[1],(float)boneidx[2],(float)boneidx[3]);
				
				}

				m_part[i].dataArray[j].vtxBuff->Unlock();
				
				//�g���{�[������������
				fread( &m_part[i].dataArray[j].usingBoneSum,sizeof( int),1,file);

				m_part[i].dataArray[j].usingBoneArray = new int[ m_part[i].dataArray[j].usingBoneSum ];

				for( int cnt = 0 ; cnt < m_part[i].dataArray[j].usingBoneSum ; cnt++ )
				{
					fread( &m_part[i].dataArray[j].usingBoneArray[cnt],sizeof( int),1,file );
				}


				//index���ǂݍ���
				fread( &m_part[i].dataArray[j].idxSum,sizeof(int),1,file );

				//�C���f�b�N�X��񐶐�
				device->CreateIndexBuffer(sizeof(WORD)*m_part[i].dataArray[j].idxSum,
													 D3DUSAGE_WRITEONLY,
													 D3DFMT_INDEX16,
													 D3DPOOL_MANAGED,
													 &m_part[i].dataArray[j].idxBuff,
													 NULL);

				WORD *index;
				m_part[i].dataArray[j].idxBuff->Lock(0,0,(void**)&index,0);

				for( int idx = 0 ; idx < m_part[i].dataArray[j].idxSum ; idx++ )
				{
					int id;
					fread( &id,sizeof(int),1,file );

					//��������
					index[idx] = id;
				}

				m_part[i].dataArray[j].idxBuff->Unlock();

		}
	
	}

	//�{�[���ǂݍ���
	fread( &m_boneSum,sizeof( int ),1,file );

	//�{�[������́H
	if( m_boneSum > 0 )
	{
		m_boneArray = new BONEDATA[ m_boneSum ];
	}
	for( int i = 0 ; i < m_boneSum ; i++ )
	{
		//�{�[��idx�擾�܂��z��ԍ��Ɠ����͂�������
		fread( &m_boneArray[ i ].boneIdx,sizeof(int),1,file );
		//�q���̐��擾
		fread( &m_boneArray[ i ].childNum,sizeof(int),1,file );

		//�q��������ΐ���
		if( m_boneArray[i].childNum != 0 )
		{
			m_boneArray[ i ].childIdxArray = new int[ m_boneArray[ i ].childNum ];
		}
		//�q��idx��ۑ�
		for( int j = 0 ; j < m_boneArray[ i ].childNum ; j++ )
		{
			fread( &m_boneArray[ i ].childIdxArray[j],sizeof( int),1,file );
		}

		//�{�[�����擾�@�ꉞ
		int len;
		fread( &len,sizeof( int ),1,file );
		fread( m_boneArray[ i ].boneName,sizeof( char ),len,file );

		//�I�t�Z�b�g�s��ǂݍ���
		for( int j = 0 ; j < 4 ; j++ )
		{
			for( int k = 0 ; k < 4 ; k++ )
			{
				float m;
				fread( &m,sizeof( float ),1,file );
				m_boneArray[ i ].offsetBoneMtx.m[j][k] = m;
			}
		}

		
		//�A�j���[�V�������J�E���g
		fread( &m_boneArray[ i ].keyMax,sizeof(int),1,file );

		if( m_boneArray[ i ].keyMax > 0 )
		{
			m_boneArray[ i ].keyFrameArray = new KEYFRAME[ m_boneArray[ i ].keyMax ];
		}
		//�A�j���[�V������񏑂�����
		for( int j = 0 ; j < m_boneArray[ i ].keyMax ; j++ )
		{
			double rot[4],scl[4],trans[4];
			float time;
			fread( rot,sizeof(double),4,file );
			fread( scl,sizeof(double),4,file );
			fread( trans,sizeof(double),4,file );
			fread( &time,sizeof( float ),1,file );

			m_boneArray[ i ].keyFrameArray[ j ].rot   = D3DXQUATERNION( (float)rot[0],(float)rot[1],(float)rot[2],(float)rot[3] );
			m_boneArray[ i ].keyFrameArray[ j ].scl   = D3DXVECTOR3( (float)scl[0],(float)scl[1],(float)scl[2] );
			m_boneArray[ i ].keyFrameArray[ j ].trans = D3DXVECTOR3( (float)trans[0],(float)trans[1],(float)trans[2] );
			m_boneArray[ i ].keyFrameArray[ j ].time  = time;
			
		}
	}

	fclose( file );

	return true;

}
