//*****************************************************************************
//
// ������ʊǗ� [standbyMaster.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "standbyMaster.h"
#include "..\..\..\common\safe.h"

#include "..\..\..\manager\manager.h"
#include "..\..\..\import\main\mainImport.h"

#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"
#include "..\..\..\score\score.h"
#include "..\..\..\effectManager\effectManager.h"
#include "..\..\..\object\player\player.h"
#include "..\..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\..\common\complement.h"

#include "..\..\..\view\light\light.h"
#include "..\..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\..\import\standby\StandbyImport.h"
#include "..\..\..\input\padX\padXManager.h"
#include "..\..\..\input\padX\padX.h"
#include "..\..\..\ggy2DAnimationManager\ggy2DAnimationManager.h"
#include "..\..\..\sound\sound.h"

#include "..\..\..\input\keyboard\keyboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
StandbyMaster::StandbyMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,StandbyImport *import,Debugproc *proc,PadXManager* padXMaster,Light *light )
{
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	m_light = light;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
StandbyMaster::~StandbyMaster(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool StandbyMaster::Create(StandbyMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						StandbyImport* import,Debugproc* debugproc,PadXManager* padXManager,Light *light)
{

	StandbyMaster* pointer = new StandbyMaster( device,objectList,updateList,drawList,import,debugproc,padXManager,light );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool StandbyMaster::Initialize(void)
{
	Polygon2D::Create( &m_back,m_device,m_objectList,m_import->texture( StandbyImport::BACK ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueTeamStandby[0].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_BLUE ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueTeamStandby[1].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_BLUE ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_redTeamStandby[0].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_RED ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_redTeamStandby[1].pol,m_device,m_objectList,m_import->texture( StandbyImport::PREPARE_RED ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_battle,m_device,m_objectList,m_import->texture( StandbyImport::BATTLE ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_redLogo.pol,m_device,m_objectList,m_import->texture( StandbyImport::SELECT_RED ),ObjectBase::TYPE_2D );
	Polygon2D::Create( &m_blueLogo.pol,m_device,m_objectList,m_import->texture( StandbyImport::SELECT_BLUE ),ObjectBase::TYPE_2D );



	MainImport* mainImport = Manager::mainImport();
	Ggy2DAnimationManager::Create( &m_ggyBlueAnimManager,m_device,m_objectList,m_updateList,m_drawListManager );

	m_ggyBlueAnimManager->SetTexture( 0,mainImport->texture(MainImport::GGYBLUE_WAIT) );
	m_ggyBlueAnimManager->SetTexture( 8,mainImport->texture(MainImport::GGYBLUE_POSE) );
	m_ggyBlueAnimManager->pos(D3DXVECTOR3( 330,420,0 ));
	m_ggyBlueAnimManager->scl(D3DXVECTOR3( 320,256,0 ));

	m_ggyBlueAnimManager->StartAnimation(0,true);

	Ggy2DAnimationManager::Create( &m_ggyRedAnimManager,m_device,m_objectList,m_updateList,m_drawListManager );

	m_ggyRedAnimManager->SetTexture( 0,mainImport->texture(MainImport::GGYRED_WAIT) );
	m_ggyRedAnimManager->SetTexture( 8,mainImport->texture(MainImport::GGYRED_POSE) );
	m_ggyRedAnimManager->pos(D3DXVECTOR3( 950,420,0 ));
	m_ggyRedAnimManager->scl(D3DXVECTOR3( 320,256,0 ));

	m_ggyRedAnimManager->StartAnimation(0,true);

	//�������΍�
	m_ggyRedAnimManager->Update();
	m_ggyBlueAnimManager->Update();



	//�w�i�����N
	m_updateList->Link( m_back );
	m_drawListManager->Link( m_back,0,Shader::PAT_2D );

	//�X�^���o�C�V�[�g�����N
	m_updateList->Link( m_blueTeamStandby[0].pol );
	m_drawListManager->Link( m_blueTeamStandby[0].pol,2,Shader::PAT_2D );
	m_updateList->Link( m_blueTeamStandby[1].pol );
	m_drawListManager->Link( m_blueTeamStandby[1].pol,2,Shader::PAT_2D );

	m_updateList->Link( m_redTeamStandby[0].pol );
	m_drawListManager->Link( m_redTeamStandby[0].pol,2,Shader::PAT_2D );
	m_updateList->Link( m_redTeamStandby[1].pol );
	m_drawListManager->Link( m_redTeamStandby[1].pol,2,Shader::PAT_2D );

	//�������S�����N
	m_updateList->Link( m_battle );
	m_drawListManager->Link( m_battle,3,Shader::PAT_2D );

	//���S
	m_updateList->Link( m_blueLogo.pol );
	m_drawListManager->Link( m_blueLogo.pol,2,Shader::PAT_2D );
	m_updateList->Link( m_redLogo.pol );
	m_drawListManager->Link( m_redLogo.pol,2,Shader::PAT_2D );

	//�X�P�[�����O�w��(�Œ�)
	m_back->scl( SCREEN_WIDTH,SCREEN_HEIGHT,1 );

	//�ʒu�w��(�Œ�)
	m_back->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0 );

	//�X�^���o�C
	m_blueTeamStandby[0].pol->scl( 0.0f,0.0f,1.0f );
	m_blueTeamStandby[1].pol->scl( 0.0f,0.0f,1.0f );

	m_redTeamStandby[0].pol->scl( 0.0f,0.0f,1.0f );
	m_redTeamStandby[1].pol->scl( 0.0f,0.0f,1.0f );

	//235  593
	//452  593
	//832  593
	//1059 593
	m_blueTeamStandby[0].pol->pos( 235.0f,593.0f,0.0f );
	m_blueTeamStandby[1].pol->pos( 452.0f,593.0f,0.0f );
	m_redTeamStandby[0].pol->pos( 832.0f,593.0f,0.0f );
	m_redTeamStandby[1].pol->pos( 1059.0f,593.0f,0.0f );

	//�������S
	m_battle->scl(0,0,1 );
	m_battle->pos( SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0 );

	//���S
	m_blueLogo.pol->pos( 340.0f,689.0f,0.0f);
	m_blueLogo.pol->scl(417.0f,59.0f,1.0f );
	m_blueLogo.pol->texcoord(0,0.0f,0.0f);
	m_blueLogo.pol->texcoord(1,1.0f,0.111f);
	m_blueLogo.pol->texcoord(2,0.0f,0.0f);
	m_blueLogo.pol->texcoord(3,1.0f,0.111f);
	m_blueLogo.uv[0] = D3DXVECTOR2(0.0f,0.0f);
	m_blueLogo.uv[1] = D3DXVECTOR2(1.0f,0.0f);
	m_blueLogo.uv[2] = D3DXVECTOR2(0.0f,0.111f);
	m_blueLogo.uv[3] = D3DXVECTOR2(1.0f,0.111f);
	m_blueLogo.offsetUv = D3DXVECTOR2(0.0f,0.111f);
	m_blueLogo.animSum = 9;
	m_blueLogo.time = 0;

	m_redLogo.pol->pos(944.0f,689.0f,0.0f);
	m_redLogo.pol->scl(417.0f,59.0f,1.0f);
	m_redLogo.pol->texcoord(0,0.0f,0.0f);
	m_redLogo.pol->texcoord(1,1.0f,0.111f);
	m_redLogo.pol->texcoord(2,0.0f,0.0f);
	m_redLogo.pol->texcoord(3,1.0f,0.111f);
	m_redLogo.uv[0] = D3DXVECTOR2(0.0f,0.0f);
	m_redLogo.uv[1] = D3DXVECTOR2(1.0f,0.111f);
	m_redLogo.uv[2] = D3DXVECTOR2(0.0f,0.0f);
	m_redLogo.uv[3] = D3DXVECTOR2(1.0f,0.111f);
	m_redLogo.offsetUv = D3DXVECTOR2(0.0f,0.111f);

	//������
	m_blueTeamStandby[0].time = _compFrame;
	m_blueTeamStandby[1].time = _compFrame;

	m_redTeamStandby[0].time = _compFrame;
	m_redTeamStandby[1].time = _compFrame;

	// �p�b�h�m�F
	m_blueTeamStandby[0].isStandby = false;

	if(m_padXManager->pad(1)->conected())
		m_blueTeamStandby[1].isStandby = false;
	else
		m_blueTeamStandby[1].isStandby = true;

	if(m_padXManager->pad(2)->conected())
		m_redTeamStandby[0].isStandby = false;
	else
		m_redTeamStandby[0].isStandby = true;

	if(m_padXManager->pad(3)->conected())
		m_redTeamStandby[1].isStandby = false;
	else
		m_redTeamStandby[1].isStandby = true;

	m_compTime = 0;

	//�f�o�b�N�p�@�e�`�[���̃X�^���o�C�����Ă������[����
	if( m_blueTeamStandby[0].isStandby && m_blueTeamStandby[1].isStandby )
	{
		m_ggyBlueAnimManager->StartAnimation(8,false);
	}

	if( m_redTeamStandby[0].isStandby && m_redTeamStandby[1].isStandby )
	{
		m_ggyRedAnimManager->StartAnimation(8,false);
	}

	m_phase = PHASE_WAIT;
	return true;
}

//=============================================================================
// �I��
//=============================================================================
void StandbyMaster::Finalize(void)
{
	m_blueTeamStandby[0].isStandby = false;
	m_blueTeamStandby[0].time = 0;
	m_blueTeamStandby[1].isStandby = false;
	m_blueTeamStandby[1].time = 0;

	m_redTeamStandby[0].isStandby = false;
	m_redTeamStandby[0].time = 0;
	m_redTeamStandby[1].isStandby = false;
	m_redTeamStandby[1].time = 0;

	SafeFinalizeDelete( m_ggyRedAnimManager );
	SafeFinalizeDelete( m_ggyBlueAnimManager );
}

//=============================================================================
// �X�V
//=============================================================================
bool StandbyMaster::Update(void)
{

	D3DXVECTOR3 battleScl;

	int animNum = (int)( m_blueLogo.time / 2 ) % m_blueLogo.animSum;

	D3DXVECTOR2 offset = m_blueLogo.offsetUv * (float)animNum;

	m_blueLogo.time++;

	if( animNum >= m_blueLogo.animSum-1 )
	{
		m_blueLogo.time--;
	}

	m_blueLogo.pol->texcoord(0,m_blueLogo.uv[0].x + offset.x,m_blueLogo.uv[0].y + offset.y );
	m_blueLogo.pol->texcoord(1,m_blueLogo.uv[1].x + offset.x,m_blueLogo.uv[1].y + offset.y );
	m_blueLogo.pol->texcoord(2,m_blueLogo.uv[2].x + offset.x,m_blueLogo.uv[2].y + offset.y );
	m_blueLogo.pol->texcoord(3,m_blueLogo.uv[3].x + offset.x,m_blueLogo.uv[3].y + offset.y );

	m_redLogo.pol->texcoord(0,m_blueLogo.uv[0].x + offset.x,m_blueLogo.uv[0].y + offset.y );
	m_redLogo.pol->texcoord(1,m_blueLogo.uv[1].x + offset.x,m_blueLogo.uv[1].y + offset.y );
	m_redLogo.pol->texcoord(2,m_blueLogo.uv[2].x + offset.x,m_blueLogo.uv[2].y + offset.y );
	m_redLogo.pol->texcoord(3,m_blueLogo.uv[3].x + offset.x,m_blueLogo.uv[3].y + offset.y );

	switch( m_phase )
	{
	case PHASE_WAIT:
		//�P�t���O�̃X�^���o�C���ێ�
		m_blueTeamStandby[0].isPrevStandby = m_blueTeamStandby[0].isStandby;
		m_blueTeamStandby[1].isPrevStandby = m_blueTeamStandby[1].isStandby;
		m_redTeamStandby[0].isPrevStandby = m_redTeamStandby[0].isStandby;
		m_redTeamStandby[1].isPrevStandby = m_redTeamStandby[1].isStandby;

		//�L�[����@��
		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_padXManager->pad(i)->buttonTrigger( XINPUT_GAMEPAD_START ) 
			|| (i==0 && m_keyboard->trigger(DIK_RETURN)))
			{
				m_blueTeamStandby[i].isStandby = true;
				Sound::Play( Sound::SE_DRUM );
			}
			else if( m_padXManager->pad(i)->buttonTrigger( XINPUT_GAMEPAD_BACK ) )
			{
				m_blueTeamStandby[i].isStandby = false;

			}
		}


		//�L�[����@��
		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_padXManager->pad(i+2)->buttonTrigger( XINPUT_GAMEPAD_START ) )
			{
				m_redTeamStandby[i].isStandby = true;
				Sound::Play( Sound::SE_DRUM );
			}
			else if( m_padXManager->pad(i+2)->buttonTrigger( XINPUT_GAMEPAD_BACK ) )
			{
				m_redTeamStandby[i].isStandby = false;
			}
		}

		bool isDiffStandby[4];
		isDiffStandby[0] = false;
		isDiffStandby[1] = false;
		isDiffStandby[2] = false;
		isDiffStandby[3] = false;

		//�󋵂��ς��΍ĕ⊮
		if( m_blueTeamStandby[0].isPrevStandby != m_blueTeamStandby[0].isStandby )
		{
			m_blueTeamStandby[0].time = 0;
			isDiffStandby[0] = true;
		}
			
		if( m_blueTeamStandby[1].isPrevStandby != m_blueTeamStandby[1].isStandby )
		{
			m_blueTeamStandby[1].time = 0;
			isDiffStandby[1] = true;
		}

		if( m_redTeamStandby[0].isPrevStandby != m_redTeamStandby[0].isStandby )
		{
			m_redTeamStandby[0].time = 0;
			isDiffStandby[2] = true;
		}

		if( m_redTeamStandby[1].isPrevStandby != m_redTeamStandby[1].isStandby )
		{
			m_redTeamStandby[1].time = 0;
			isDiffStandby[3] = true;
		}

		if( isDiffStandby[0] || isDiffStandby[1] )
		{
			if( m_blueTeamStandby[0].isStandby && m_blueTeamStandby[1].isStandby )
			{
				m_ggyBlueAnimManager->StartAnimation(8,false );
				Sound::Play( Sound::SE_SELECT_VOICEA );
			}
			else
			{
				m_ggyBlueAnimManager->StartAnimation(0,true );
			}

		}

		if( isDiffStandby[2] || isDiffStandby[3] )
		{
			if( m_redTeamStandby[0].isStandby && m_redTeamStandby[1].isStandby )
			{
				m_ggyRedAnimManager->StartAnimation(8,false );
				Sound::Play( Sound::SE_SELECT_VOICEB );
			}
			else
			{
				m_ggyRedAnimManager->StartAnimation(0,true );
			}
		}



		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_blueTeamStandby[i].isStandby )
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1.0f ),0,(float)_compFrame,m_blueTeamStandby[i].time,Cube );
				m_blueTeamStandby[i].pol->scl(scl);

				m_blueTeamStandby[i].time++;

				if( m_blueTeamStandby[i].time > _compFrame )
				{
					m_blueTeamStandby[i].time = _compFrame;
				}
			}
			else
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1.0f ),D3DXVECTOR3( 0.0f,0.0f,0.0f ),0,(float)_compFrame,m_blueTeamStandby[i].time,Cube );
				m_blueTeamStandby[i].pol->scl(scl);

				m_blueTeamStandby[i].time++;

				if( m_blueTeamStandby[i].time > _compFrame )
				{
					m_blueTeamStandby[i].time = _compFrame;
				}
			}
		}

		for( int i = 0 ; i < 2 ; i++ )
		{
			if( m_redTeamStandby[i].isStandby )
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1 ),0,(float)_compFrame,m_redTeamStandby[i].time,Cube );
				m_redTeamStandby[i].pol->scl(scl);

				m_redTeamStandby[i].time++;

				if( m_redTeamStandby[i].time > _compFrame )
				{
					m_redTeamStandby[i].time = _compFrame;
				}
			}
			else
			{
				D3DXVECTOR3 scl;
				scl = LerpVec3( D3DXVECTOR3( 91.0f*0.5f,269.0f*0.5f,1 ),D3DXVECTOR3( 0.0f,0.0f,0.0f ),0,(float)_compFrame,m_redTeamStandby[i].time,Cube );
				m_redTeamStandby[i].pol->scl(scl);

				m_redTeamStandby[i].time++;

				if( m_redTeamStandby[i].time > _compFrame )
				{
					m_redTeamStandby[i].time = _compFrame;
				}
			}
		}

		//�����o�[�S�Ă����������Ȃ�
		if( m_blueTeamStandby[0].isStandby && m_blueTeamStandby[1].isStandby && m_redTeamStandby[0].isStandby && m_redTeamStandby[1].isStandby
			&& m_blueTeamStandby[0].time >= _compFrame && m_blueTeamStandby[1].time >= _compFrame 
			&& m_redTeamStandby[0].time >= _compFrame  && m_redTeamStandby[1].time >= _compFrame)
		{
			m_phase = PHASE_MOTION;
			//m_ggyRedAnimManager->StartAnimation(8,false);
			//m_ggyBlueAnimManager->StartAnimation(8,false);
			Sound::Play( Sound::SE_SELECT_VOICE_IZA );
		}

		break;

	case PHASE_MOTION:

		//�Ԃ��������t���[�����Ȃ̂ŐԂŔ��肵�Ƃ�
		if( m_ggyRedAnimManager->isEndAnimation() )
		{
			m_phase = PHASE_STANDBY;
			Sound::Play( Sound::SE_SELECT_VOICE_BATTLE );
		}

		
		break;

	case PHASE_STANDBY:

		battleScl = LerpVec3( D3DXVECTOR3( 781.0f*4.0f,413.0f*4.0f,1.0f ),D3DXVECTOR3( 781.0f/2.0f,413.0f/2.0f,0.0f ),0,_compFrame*2,m_compTime,Cube );

		m_battle->scl(battleScl);

		m_compTime++;

		if( m_compTime > _compFrame*2 )
		{
			m_compTime = _compFrame*2;
			m_phase = PHASE_STANDBY_WAIT;
			m_compTime = 0;
		}

		break;

	case PHASE_STANDBY_WAIT:

		m_compTime++;

		if( m_compTime >= _compFrame*2 )
		{
			m_compTime = 0;
			m_phase = PHASE_NEXTSCENE;
		}

		break;
		
	case PHASE_NEXTSCENE:

		return true;

		break;

	}

	m_ggyRedAnimManager->Update();
	m_ggyBlueAnimManager->Update();

	return false;
}

//=============================================================================
// �`��
//=============================================================================
void StandbyMaster::Draw(void)
{
}
