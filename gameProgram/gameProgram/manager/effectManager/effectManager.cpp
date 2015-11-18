//*****************************************************************************
//
// �G�t�F�N�g�}�l�[�W���N���X [effectManager.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "effectManager.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"



//�⊮�p
float Cube( float T ){ return T * T * ( 3.0f - 2.0f * T ); }
float Linear( float T ){ return T ; }
float EaseIn( float T ){return T*T;}
float EaseOut( float T ){ return T*(2-T);}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
EffectManager::EffectManager(InstancingBillboard *insBill)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	 m_insBill = insBill ;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
EffectManager::~EffectManager(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool EffectManager::Create(EffectManager** outPointer,InstancingBillboard *insBill )
{
	EffectManager* pointer = new EffectManager(insBill);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool EffectManager::Initialize(void)
{
	//----------------------------
	// �R�����g
	//----------------------------

	//�����ł���r���{�[�h�����ő�p�[�e�B�N�����ƂȂ�
	m_particleMax = m_insBill->GetPolygonMax();

	m_databaseCursol = 0;

	//�G�t�F�N�g�z�񐶐�
	m_particleArray = new PARTICLE[ m_particleMax ];

	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		m_particleArray[ i ].isDelete = true;
	}

	m_cursol = 0;

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void EffectManager::Finalize(void)
{
	if( m_particleArray != nullptr )
	{
		delete []m_particleArray;
		m_particleArray = nullptr;
	}

	for( int i = 0 ; i < m_databaseCursol ; i++ )
	{
		delete []m_effectDatabase[i].particleArray;
		m_databaseCursol = 0;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void EffectManager::Update(void)
{
	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		//�G�t�F�N�g���S

		if( !m_particleArray[i].isDelete )
		{

			if( m_particleArray[i].lifeSpan <= 0 )
			{
				m_particleArray[i].polygon->isDelete = true;
				m_particleArray[i].isDelete = true;
				continue;
			}

			D3DXVECTOR3 pos,scl;
			D3DXVECTOR4 col;

			switch( m_particleArray[i].mode )
			{
				case EffectManager::COMP_MODE_LINE:
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Linear );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Linear );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Linear );
					break;																														  
				case EffectManager::COMP_MODE_EASEIN:																							  
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseIn );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseIn );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseIn );
																																				  
					break;																														  
				case EffectManager::COMP_MODE_EASEOUT:																							  
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseOut );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseOut );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseOut );
																																				  
					break;																														  
				case EffectManager::COMP_MODE_CUBE:																								  
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Cube );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Cube );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Cube );
					break;
			}

			m_particleArray[i].compErase++;

			if( m_particleArray[i].compErase > m_particleArray[i].compFrame )
			{
				m_particleArray[i].compErase = m_particleArray[i].compFrame;
				m_particleArray[i].lifeSpan = 0;
			}

			//�A�j���[�V�����X�V
			D3DXVECTOR2 offset = m_particleArray[i].anim.startUvOffset;
			int anim_cnt = 0;
			//�A�j���[�V�����������
			if( m_particleArray[i].anim.animSum > 0 )
			{
				//�P�A�j���[�V�������؂�ւ��t���[�������Z�o
				int change_frame = m_particleArray[i].animFrame / m_particleArray[i].anim.animSum;

				m_particleArray[i].animErase++;

				//�������ŃA�j���\�V�����J�E���g������
				anim_cnt = m_particleArray[i].animErase / change_frame;

				offset.x += 1 * ( anim_cnt % m_particleArray[i].anim.animSum );
			}
			//�K�p
			m_particleArray[i].polygon->pos = pos;
			m_particleArray[i].polygon->col = D3DXCOLOR( col.x,col.y,col.z,col.w );
			m_particleArray[i].polygon->scl = scl;
			m_particleArray[i].polygon->uvOffset = offset;

		}

	}
}


//---------------------------------------------------------------------------------
//�⊮
//---------------------------------------------------------------------------------

D3DXVECTOR3 EffectManager::Lerp( const D3DXVECTOR3 &st,const D3DXVECTOR3 &ed,float min,float max,float elapsed,float (*func)(float) )
{
	float t = ( elapsed - min ) / ( max - min );// ���Ԃ�}��ϐ���

	if( t < 0.0 )
	{
		t = 0;
	}

	if( t > 1.0 )
	{
		t = 1;
	}

	//�e�⊮�֐��Ń��[�g������
	float rate = (*func)(t);

	return st * ( 1.0f - rate ) + ed * rate;
}

D3DXVECTOR4 EffectManager::Lerp( const D3DXVECTOR4 &st,const D3DXVECTOR4 &ed,float min,float max,float elapsed,float (*func)(float)  )
{
	float t = ( elapsed - min ) / ( max - min );// ���Ԃ�}��ϐ���

	if( t < 0.0 )
	{
		t = 0;
	}

	if( t > 1.0 )
	{
		t = 1;
	}

	//�e�⊮�֐��Ń��[�g������
	float rate = (*func)(t);

	return st * ( 1.0f - rate ) + ed * rate;
}

//-------------------------------------------------------------------------------------------------------
//�G�t�F�N�g����
//-------------------------------------------------------------------------------------------------------

void EffectManager::AddParticle(const D3DXVECTOR3 &posSt,const D3DXVECTOR3 &sclSt,const D3DXVECTOR4 &colSt,
								const D3DXVECTOR3 &posEd,const D3DXVECTOR3 &sclEd,const D3DXVECTOR4 &colEd,
								const int compFrame,const int life,const int animSum,const int animFrame,const int offsetX,const int offsetY,
								const COMPLETIONMODE compMode )
{

	int idx = -1;

	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		if( m_particleArray[ m_cursol ].isDelete )
		{
			idx = m_cursol;
			break;
		}


		m_cursol++;

		if( m_cursol >= m_particleMax-1 )
		{
			m_cursol = 0;
		}
	}

	//�C���X�^���V���O���̃G���[�`�F�b�N���Ȃ��̂̓p�[�e�B�N���ő吔 = �|���S��������
	if( idx != -1 )
	{
		PARTICLE p;

		p.key[0].pos = posSt;
		p.key[1].pos = posEd;
		p.key[0].scl = sclSt;
		p.key[1].scl = sclEd;
		p.key[0].col = colSt;
		p.key[1].col = colEd;
		p.compFrame = compFrame;
		p.lifeSpan = life;
		p.anim.animSum = animSum;
		p.animFrame = animFrame;
		p.mode = compMode;
		p.anim.startUvOffset.x = offsetX;
		p.anim.startUvOffset.y = offsetY;
		p.isDelete = false;

		p.compErase = 0;
		p.animErase = 0;
		
		p.polygon = m_insBill->LentInsPolygonData();
		p.polygon->pos = p.key[0].pos;
		p.polygon->rot = D3DXVECTOR3(0,0,0);
		p.polygon->scl = p.key[0].scl;
		p.polygon->col = D3DXCOLOR( colSt.x,colSt.y,colSt.z,colSt.w );
		p.polygon->uvOffset = p.anim.startUvOffset;
		p.polygon->isDelete = false;
		m_particleArray[ idx ] = p;
	}
}

void EffectManager::AddEffectFromDataBase( const int idx,const D3DXVECTOR3 pos )
{

	//�֐��Ăт܂�����͑����C������
	for( int i = 0 ; i < m_effectDatabase[idx].particleSum ; i++ )
	{
		int lentidx = -1;

		for( int j = 0 ; j < m_particleMax ; j++ )
		{
			if( m_particleArray[ m_cursol ].isDelete )
			{
				lentidx = m_cursol;
				break;
			}


			m_cursol++;

			if( m_cursol >= m_particleMax-1 )
			{
				m_cursol = 0;
			}
		}

		//���Ȃ����Ă��Ƃ͂����󂫂Ȃ���ŏI��
		if( lentidx == -1 )
		{
			break;
		}

		//�Z�b�g
		m_particleArray[ lentidx ].isDelete     = false;
		m_particleArray[ lentidx ].animErase    = 0;
		m_particleArray[ lentidx ].compErase    = 0;
		m_particleArray[ lentidx ].anim.animSum         = m_effectDatabase[ idx ].particleArray[i].anim.animSum;
		m_particleArray[ lentidx ].anim.startUvOffset   = m_effectDatabase[ idx ].particleArray[i].anim.startUvOffset;
		m_particleArray[ lentidx ].animFrame            = m_effectDatabase[ idx ].particleArray[i].animFrame;
		m_particleArray[ lentidx ].compFrame            = m_effectDatabase[ idx ].particleArray[i].compFrame;
		m_particleArray[ lentidx ].key[0].pos           = m_effectDatabase[ idx ].particleArray[i].key[0].pos + pos;
		m_particleArray[ lentidx ].key[0].scl           = m_effectDatabase[ idx ].particleArray[i].key[0].scl;
		m_particleArray[ lentidx ].key[0].col           = m_effectDatabase[ idx ].particleArray[i].key[0].col;
		m_particleArray[ lentidx ].key[1].pos           = m_effectDatabase[ idx ].particleArray[i].key[1].pos + pos;
		m_particleArray[ lentidx ].key[1].scl           = m_effectDatabase[ idx ].particleArray[i].key[1].scl;
		m_particleArray[ lentidx ].key[1].col           = m_effectDatabase[ idx ].particleArray[i].key[1].col;
		m_particleArray[ lentidx ].lifeSpan             = m_effectDatabase[ idx ].particleArray[i].lifeSpan;
		m_particleArray[ lentidx ].mode                 = m_effectDatabase[ idx ].particleArray[i].mode;
		m_particleArray[ lentidx ].polygon              = m_insBill->LentInsPolygonData();
		m_particleArray[ lentidx ].polygon->pos = m_particleArray[ lentidx ].key[0].pos;
		m_particleArray[ lentidx ].polygon->scl = m_particleArray[ lentidx ].key[0].scl;
		m_particleArray[ lentidx ].polygon->col = D3DXCOLOR(m_particleArray[ lentidx ].key[0].col.x,m_particleArray[ lentidx ].key[0].col.y,
														m_particleArray[ lentidx ].key[0].col.z,m_particleArray[ lentidx ].key[0].col.w );
		m_particleArray[ lentidx ].polygon->uvOffset = m_particleArray[ lentidx ].anim.startUvOffset;
		m_particleArray[ lentidx ].polygon->isDelete = false;

	}
}

//-------------------------------------------------------------------------------------------------------------
//�e�N�X�`�����[�h
//-------------------------------------------------------------------------------------------------------------

void EffectManager::LoadTexture( const char *filePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize )
{
	m_insBill->LoadTexture( filePath );
	m_insBill->SetVtxBuffTexInfo( texSize,oneSize );
}

//-------------------------------------------------------------------------------------------------------------
//�e�N�X�`�����
//-------------------------------------------------------------------------------------------------------------

void EffectManager::SetTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize )
{
	m_insBill->SetVtxBuffTexInfo( texSize,oneSize );
}

//------------------------------------------------------------------------------------------------------------
//�J�����ʒu�Z�b�g
//------------------------------------------------------------------------------------------------------------

void EffectManager::SetCamPos( const D3DXVECTOR3 &camPos )
{
	m_insBill->SetCameraPos( camPos );
}

//-----------------------------------------------------------------------------------------------------------
//�G�t�F�N�g�t�@�C�����f�[�^�x�[�X�Ƀ��[�h
//-----------------------------------------------------------------------------------------------------------

void EffectManager::LoadEffectData( const char *filePath )
{
	FILE *file = NULL;
	fopen_s( &file,filePath,"rb");
	int size;

	//�O���[�v���ۑ�
	fread(&size,sizeof(int),1,file );

	int byte = sizeof( D3DXVECTOR3 )*2+ sizeof( D3DXVECTOR2 ) * 3 + sizeof( D3DXVECTOR4 ) * 2 + sizeof( int )*4;
	int create_sum = 0;

	//���G�t�F�N�g�����J�E���g
	for( int group = 0; group < size ; group++ )
	{
		//�����G�t�F�N�g���ۑ�
		int buf;
		fread( &buf,sizeof(int),1,file );
		create_sum += buf;

		for( int i = 0 ; i < buf; i++ )
		{
			fseek(file,byte,SEEK_CUR );
		}

	}

	fseek( file,0,SEEK_SET );

	//�O���[�v���ۑ�
	fread(&size,sizeof(int),1,file );

	m_effectDatabase[m_databaseCursol ].particleArray = new PARTICLE[ create_sum ];
	m_effectDatabase[m_databaseCursol ].particleSum = create_sum;

	int cnt = 0;

	for( int group = 0; group < size ; group++ )
	{
		//�����G�t�F�N�g���ۑ�
		int buf;
		fread( &buf,sizeof(int),1,file );

		for( int i = 0 ; i < buf; i++ )
		{
			D3DXVECTOR2 scl_st,scl_ed;
			int life;
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[0].pos,sizeof( D3DXVECTOR3 ),1,file );
			fread( &scl_st,sizeof( D3DXVECTOR2 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[0].col,sizeof( D3DXVECTOR4 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[1].pos,sizeof( D3DXVECTOR3 ),1,file );
			fread( &scl_ed,sizeof( D3DXVECTOR2 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[1].col,sizeof( D3DXVECTOR4 ),1,file );
			fread( &life,  sizeof( int ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].mode,sizeof( int ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].anim.startUvOffset,sizeof( D3DXVECTOR2 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].anim.animSum,sizeof( int ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].animFrame,sizeof( int ),1,file );

			//
			m_effectDatabase[m_databaseCursol].particleArray[cnt].lifeSpan = life;
			m_effectDatabase[m_databaseCursol].particleArray[cnt].compFrame = m_effectDatabase[m_databaseCursol].particleArray[cnt].lifeSpan;
			m_effectDatabase[m_databaseCursol].particleArray[cnt].key[0].scl = D3DXVECTOR3( scl_st.x,scl_st.y,1 );
			m_effectDatabase[m_databaseCursol].particleArray[cnt].key[1].scl = D3DXVECTOR3( scl_ed.x,scl_ed.y,1 );

			cnt++;
		}

	}

	fclose( file );

	m_databaseCursol++;
}

//---------------------------------------------------------------------------------------
//�I�v�V�����Z�b�g
//---------------------------------------------------------------------------------------

void EffectManager::SetOption( InstancingBillboard::OPTION option )
{
	m_insBill->SetOption( option );
}


// EOF