//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [AudienceManager.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "audienceManager.h"
#include "..\objectBase\instancingBillboard\instancingBillboard.h"
#include "..\common\complement\complement.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include <math.h>
//=============================================================================
//�X�^�e�B�b�N
//=============================================================================

AudienceStateStayRedTeam *AudienceManager::m_stateStayRed = nullptr;
AudienceStateStayBlueTeam *AudienceManager::m_stateStayBlue = nullptr;
AudienceStateMoveRedTeam *AudienceManager::m_stateRedMove = nullptr;
AudienceStateMoveBlueTeam *AudienceManager::m_stateBlueMove = nullptr;
int AudienceManager::m_refCnt = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
AudienceManager::AudienceManager( InstancingBillboard *insBill,const int scoreMax )
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_insBill = insBill;
	m_audienceSum = scoreMax;

	//�X�^�e�B�b�N�����o������ĂȂ���΍��
	if( m_stateStayRed == nullptr )
	{
		m_stateStayRed = new AudienceStateStayRedTeam;
	}

	if( m_stateStayBlue == nullptr )
	{
		m_stateStayBlue = new AudienceStateStayBlueTeam;
	}

	if( m_stateRedMove == nullptr )
	{
		m_stateRedMove = new AudienceStateMoveRedTeam;
	}

	if( m_stateBlueMove == nullptr )
	{
		m_stateBlueMove = new AudienceStateMoveBlueTeam;
	}

	//�Q�ƃJ�E���^�����Z
	m_refCnt++;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
AudienceManager::~AudienceManager(void)
{
	m_refCnt--;

	//�Q�Ƃ��Ă���z�����Ȃ���Ή��
	if( m_refCnt <= 0 )
	{
		if( m_stateStayRed != nullptr )
		{
			delete m_stateStayRed;
			m_stateStayRed = nullptr;
		}

		if( m_stateStayBlue != nullptr )
		{
			delete m_stateStayBlue;
			m_stateStayBlue = nullptr;
		}

		if( m_stateRedMove != nullptr )
		{
			delete m_stateRedMove;
			m_stateRedMove = nullptr;
		}

		if( m_stateBlueMove != nullptr )
		{
			delete m_stateBlueMove;
			m_stateBlueMove = nullptr;
		}
	}
}

//=============================================================================
// ����
//=============================================================================
bool AudienceManager::Create(AudienceManager** outPointer,LPDIRECT3DDEVICE9 device, ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						int priority  , ObjectBase::OBJECT_TYPE type,const char* textureLoadPath,const int scoreMax,
						const D3DXVECTOR3 &leftPosLimRed,const D3DXVECTOR3 &rightPosLimRed,
						const D3DXVECTOR3 &leftPosLimBlue,const D3DXVECTOR3 &rightPosLimBlue)
{
	//�C���X�^���V���O�r���{�[�h���쐬
	D3DXVECTOR2 texSize = D3DXVECTOR2(1,1);
	D3DXVECTOR2 oneSize = D3DXVECTOR2(1,1);
	InstancingBillboard *bill ;
	InstancingBillboard::Create( &bill,device,objectList,priority,type,scoreMax,textureLoadPath,texSize,oneSize );

	updateList->Link( bill );
	drawList->Link( bill,0,Shader::PAT_INS);

	//�{�̐���
	AudienceManager* pointer = new AudienceManager( bill,scoreMax );

	pointer->SetRedTeamPosLimit( leftPosLimRed,rightPosLimRed );
	pointer->SetBlueTeamPosLimit( leftPosLimBlue,rightPosLimBlue );
	if(!pointer->Initialize())
	{
		return false;
	}

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool AudienceManager::Initialize(void)
{
	//----------------------------
	//
	//----------------------------
	m_redTeamMoveBuff = 0;
	m_blueTeamMoveBuff = 0;
	
	//----------------------------
	// �z�񐶐�
	//----------------------------
	m_audienceArray = new AUDIENCEDATA[ m_audienceSum ];

	//�؂�Ă���
	for( int i = 0 ; i < m_audienceSum ; i++ )
	{
		m_audienceArray[i].poly = m_insBill->LentInsPolygonData();
	}

	for( int i = 0 ; i < m_audienceSum/2 ; i++ )
	{
		m_audienceArray[i].compTime = 100;
		m_audienceArray[i].state = m_stateStayBlue;
		m_audienceArray[i].stPos = D3DXVECTOR3(0,0,0);
		m_audienceArray[i].edPos = D3DXVECTOR3(0,0,0);
		m_audienceArray[i].pos = D3DXVECTOR3( 0,50,-1000 );
		m_audienceArray[i].scl = D3DXVECTOR3(50,100,0 );
		m_audienceArray[i].team = TEAM_BLUE;
		m_audienceArray[i].col  = D3DXCOLOR(0,0,1,1);
		m_audienceArray[i].poly->pos = m_audienceArray[i].pos;
		m_audienceArray[i].poly->scl = m_audienceArray[i].scl;
		m_audienceArray[i].poly->col = m_audienceArray[i].col;
		m_audienceArray[i].poly->uvOffset = D3DXVECTOR2(0,0);
		m_audienceArray[i].state->Init( &m_audienceArray[i] );
	}

	for( int i = m_audienceSum/2 ; i < m_audienceSum ; i++ )
	{
		m_audienceArray[i].compTime = 100;
		m_audienceArray[i].state = m_stateStayRed;
		m_audienceArray[i].stPos = D3DXVECTOR3(0,0,0);
		m_audienceArray[i].edPos = D3DXVECTOR3(0,0,0);
		m_audienceArray[i].pos = D3DXVECTOR3( 150,50,-1000 );
		m_audienceArray[i].scl = D3DXVECTOR3(50,100,0 );
		m_audienceArray[i].team = TEAM_RED;
		m_audienceArray[i].col  = D3DXCOLOR(1,0,0,1);
		m_audienceArray[i].poly->pos = m_audienceArray[i].pos;
		m_audienceArray[i].poly->scl = m_audienceArray[i].scl;
		m_audienceArray[i].poly->col = m_audienceArray[i].col;
		m_audienceArray[i].poly->uvOffset = D3DXVECTOR2(0,0);
		m_audienceArray[i].state->Init( &m_audienceArray[i] );
	}

	m_insBill->SetOption( InstancingBillboard::OPTION(false,false,true ));

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void AudienceManager::Finalize(void)
{
	if( m_audienceArray != nullptr )
	{
		delete []m_audienceArray;
		m_audienceArray = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void AudienceManager::Update(void)
{
	ReduceMoveBuff();

	for( int i = 0 ; i < m_audienceSum ; i++ )
	{
		m_audienceArray[i].state->Update(&m_audienceArray[i]);
	}
}

//=============================================================================
//
//=============================================================================

void AudienceManager::ReduceMoveBuff()
{
	if( m_redTeamMoveBuff <= 0 && m_blueTeamMoveBuff <= 0)
	{
		return;
	}

	//�ړ�����
	for( int i = 0 ; i < m_audienceSum ; i++ )
	{
		//�ړ��Ώۃ`�[���Ȃ�
		if( m_audienceArray[i].state->isMove() )
		{
			if( m_audienceArray[i].team == TEAM_BLUE && m_redTeamMoveBuff > 0)
			{
				m_audienceArray[i].team = TEAM_RED;//�`�[���𔽓]����

				//�X�e�[�g���ړ���
				m_audienceArray[i].state = m_stateRedMove;

				//Init�Őݒ�
				m_audienceArray[i].state->Init( &m_audienceArray[i] );

				m_redTeamMoveBuff--;
			}

			if( m_audienceArray[i].team == TEAM_RED && m_blueTeamMoveBuff > 0)
			{
				m_audienceArray[i].team = TEAM_BLUE;//�`�[���𔽓]����

				//�X�e�[�g���ړ���
				m_audienceArray[i].state = m_stateBlueMove;

				//Init�Őݒ�
				m_audienceArray[i].state->Init( &m_audienceArray[i] );

				m_blueTeamMoveBuff--;
			}

			if( m_redTeamMoveBuff <= 0 && m_blueTeamMoveBuff <= 0)
			{
				break;
			}

		}
	}
}

//=============================================================================
//
//=============================================================================
bool AudienceManager::MoveRedTeamFromBlueTeam( const int moveVal )
{
	int mCnt = moveVal;

	if( mCnt <= 0 )
	{
		return false;
	}

	//�ړ�����
	for( int i = 0 ; i < m_audienceSum ; i++ )
	{
		//�ړ��\���� �ړ��Ώۃ`�[���Ȃ�
		if( m_audienceArray[i].team == TEAM_BLUE && m_audienceArray[i].state->isMove() )
		{
			m_audienceArray[i].team = TEAM_RED;//�`�[���𔽓]����

			//�X�e�[�g���ړ���
			m_audienceArray[i].state = m_stateRedMove;

			//Init�Őݒ�
			m_audienceArray[i].state->Init( &m_audienceArray[i] );

			mCnt--;

			if( mCnt <= 0 )
			{
				return true;
			}

		}
	}

	m_redTeamMoveBuff += mCnt;
	return false;
}

bool AudienceManager::MoveBlueTeamFromRedTeam( const int moveVal )
{
	int mCnt = moveVal;

	if( mCnt <= 0 )
	{
		return false;
	}

	//�ړ�����
	for( int i = 0 ; i < m_audienceSum ; i++ )
	{
		//�ړ��\���� �ړ��Ώۃ`�[���Ȃ�
		if( m_audienceArray[i].team == TEAM_RED && m_audienceArray[i].state->isMove() )
		{
			m_audienceArray[i].team = TEAM_BLUE;//�`�[���𔽓]����

			//�X�e�[�g���ړ���
			m_audienceArray[i].state = m_stateBlueMove;

			//Init�Őݒ�
			m_audienceArray[i].state->Init( &m_audienceArray[i] );

			mCnt--;

			if( mCnt <= 0 )
			{
				return true;
			}

		}
	}

	m_blueTeamMoveBuff += mCnt;
	return false;
}

//====================================================================
//�e�w�c�̈ړ����E���Z�b�g
//====================================================================

void AudienceManager::SetRedTeamPosLimit( const D3DXVECTOR3 &leftLim,const D3DXVECTOR3 &rightLim)
{
	m_stateStayRed->leftLimit(leftLim);
	m_stateRedMove->leftLimit(leftLim);
	m_stateStayRed->rightLimit(rightLim);
	m_stateRedMove->rightLimit(rightLim);

}
void AudienceManager::SetBlueTeamPosLimit( const D3DXVECTOR3 &leftLim,const D3DXVECTOR3 &rightLim)
{
	m_stateStayBlue->leftLimit(leftLim);
	m_stateBlueMove->leftLimit(leftLim);
	m_stateStayBlue->rightLimit(rightLim);
	m_stateBlueMove->rightLimit(rightLim);
}
//====================================================================
//State
//====================================================================

void AudienceStateStayRedTeam::Init( AudienceManager::AUDIENCEDATA *audienceData)
{
	audienceData->eraseTime = 0;
	audienceData->eraseTimeY = 0;

	D3DXVECTOR3 end;

	float offset = audienceData->scl.x*2;

	end.x = RandRange( m_rightLimit.x,m_leftLimit.x+offset );
	end.y = RandRange( m_rightLimit.y,m_leftLimit.y );
	end.z = RandRange( m_rightLimit.z,m_leftLimit.z );

	audienceData->edPos = end;
	audienceData->stPos = audienceData->pos;
	
}

void AudienceStateStayBlueTeam::Init( AudienceManager::AUDIENCEDATA *audienceData )
{
	audienceData->eraseTime = 0;
	audienceData->eraseTimeY = 0;

	D3DXVECTOR3 end;
	float offset = audienceData->scl.x*2;
	end.x = RandRange( m_rightLimit.x-offset,m_leftLimit.x );
	end.y = RandRange( m_rightLimit.y,m_leftLimit.y );
	end.z = RandRange( m_rightLimit.z,m_leftLimit.z );

	audienceData->edPos = end;
	audienceData->stPos = audienceData->pos;
}

void AudienceStateMoveRedTeam::Init( AudienceManager::AUDIENCEDATA *audienceData )
{
	audienceData->eraseTime = 0;
	audienceData->eraseTimeY = 0;
	audienceData->stPos = audienceData->pos;

	D3DXVECTOR3 end;

	end.x = RandRange( m_rightLimit.x,m_leftLimit.x );
	end.y = RandRange( m_rightLimit.y,m_leftLimit.y );
	end.z = RandRange( m_rightLimit.z,m_leftLimit.z );

	audienceData->edPos = end;
	
}

void AudienceStateMoveBlueTeam::Init( AudienceManager::AUDIENCEDATA *audienceData )
{
	audienceData->eraseTime = 0;
	audienceData->eraseTimeY = 0;
	audienceData->stPos = audienceData->pos;

	D3DXVECTOR3 end;

	end.x = RandRange( m_rightLimit.x,m_leftLimit.x );
	end.y = RandRange( m_rightLimit.y,m_leftLimit.y );
	end.z = RandRange( m_rightLimit.z,m_leftLimit.z );

	audienceData->edPos = end;
}

void AudienceStateStayRedTeam::Update( AudienceManager::AUDIENCEDATA *audienceData )
{
	//�⊮�ړ�
	//Y����������ς������̂ŗv�f��
	audienceData->pos.x = Lerp( audienceData->stPos.x,audienceData->edPos.x,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.z = Lerp( audienceData->stPos.z,audienceData->edPos.z,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.y = Lerp( audienceData->stPos.y,audienceData->edPos.y,0,audienceData->compTime/4,audienceData->eraseTimeY,EaseIn );
	audienceData->eraseTimeY++;
	audienceData->eraseTime++;


	//�x�����⊮�I���@�㉺���[�v
	if( audienceData->eraseTimeY >= audienceData->compTime/4 )
	{
		float buf = audienceData->stPos.y;
		audienceData->stPos.y = audienceData->pos.y;
		audienceData->edPos.y = buf;
		audienceData->eraseTimeY = 0;
	}

		//�ړ��I��
	if( audienceData->eraseTime >= audienceData->compTime && audienceData->eraseTimeY == 0 )
	{
		D3DXVECTOR3 end;
		
		end.x = RandRange( m_rightLimit.x,m_leftLimit.x );
		end.y = RandRange( m_rightLimit.y,m_leftLimit.y );
		end.z = audienceData->edPos.z;

		audienceData->edPos = end;
		audienceData->stPos = audienceData->pos;
		audienceData->eraseTime = 0;
		
	}
	else if(audienceData->eraseTime >= audienceData->compTime)
	{
		audienceData->eraseTime = audienceData->compTime;
	}

	audienceData->poly->col = audienceData->col;
	audienceData->poly->pos = audienceData->pos;
}

void AudienceStateStayBlueTeam::Update( AudienceManager::AUDIENCEDATA *audienceData )
{
	//�⊮�ړ�
	//Y����������ς������̂ŗv�f��
	audienceData->pos.x = Lerp( audienceData->stPos.x,audienceData->edPos.x,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.z = Lerp( audienceData->stPos.z,audienceData->edPos.z,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.y = Lerp( audienceData->stPos.y,audienceData->edPos.y,0,audienceData->compTime/4,audienceData->eraseTimeY,EaseIn );
	audienceData->eraseTimeY++;
	audienceData->eraseTime++;


	//�x�����⊮�I���@�㉺���[�v
	if( audienceData->eraseTimeY >= audienceData->compTime/4 )
	{
		float buf = audienceData->stPos.y;
		audienceData->stPos.y = audienceData->pos.y;
		audienceData->edPos.y = buf;
		audienceData->eraseTimeY = 0;
	}


	//�ړ��I��
	if( audienceData->eraseTime >= audienceData->compTime && audienceData->eraseTimeY == 0 )
	{
		D3DXVECTOR3 end;
		
		end.x = RandRange( m_rightLimit.x,m_leftLimit.x );
		end.y = RandRange( m_rightLimit.y,m_leftLimit.y );
		end.z = audienceData->edPos.z;
		
		audienceData->edPos = end;
		audienceData->stPos = audienceData->pos;
		audienceData->eraseTime = 0;
	}
	else if(audienceData->eraseTime >= audienceData->compTime)
	{
		audienceData->eraseTime = audienceData->compTime;
	}

	audienceData->poly->col = audienceData->col;
	audienceData->poly->pos = audienceData->pos;
}

void AudienceStateMoveRedTeam::Update( AudienceManager::AUDIENCEDATA *audienceData )
{
	//�⊮�ړ�
	//Y����������ς������̂ŗv�f��
	audienceData->pos.x = Lerp( audienceData->stPos.x,audienceData->edPos.x,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.z = Lerp( audienceData->stPos.z,audienceData->edPos.z,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.y = Lerp( audienceData->stPos.y,audienceData->edPos.y,0,audienceData->compTime/4,audienceData->eraseTimeY,EaseIn );
	audienceData->eraseTimeY++;
	audienceData->eraseTime++;


	//�x�����⊮�I���@�㉺���[�v
	if( audienceData->eraseTimeY >= audienceData->compTime/4 )
	{
		float buf = audienceData->stPos.y;
		audienceData->stPos.y = audienceData->pos.y;
		audienceData->edPos.y = buf;
		audienceData->eraseTimeY = 0;
	}

	audienceData->poly->pos = audienceData->pos;

	if( audienceData->pos.x >= m_leftLimit.x )
	{
		audienceData->col = D3DXCOLOR(1,0,0,1);
		audienceData->poly->col = audienceData->col;
	}

	
	//�ړ��I��
	if( audienceData->eraseTime >= audienceData->compTime && audienceData->eraseTimeY == 0 )
	{
		audienceData->col = D3DXCOLOR(1,0,0,1);

		//�J�n�ʒu�����݈ʒu��
		audienceData->stPos = audienceData->pos;
		//xz�̏I���ʒu�͂������Ō��߂�
		audienceData->state = AudienceManager::GetStateStayRed();
		audienceData->state->Init( audienceData );

		audienceData->poly->col = audienceData->col;

	}
	else if(audienceData->eraseTime >= audienceData->compTime)
	{
		audienceData->eraseTime = audienceData->compTime;
	}
}

void AudienceStateMoveBlueTeam::Update( AudienceManager::AUDIENCEDATA *audienceData )
{
	//�⊮�ړ�
	//Y����������ς������̂ŗv�f��
	audienceData->pos.x = Lerp( audienceData->stPos.x,audienceData->edPos.x,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.z = Lerp( audienceData->stPos.z,audienceData->edPos.z,0,audienceData->compTime,audienceData->eraseTime,Cube );
	audienceData->pos.y = Lerp( audienceData->stPos.y,audienceData->edPos.y,0,audienceData->compTime/4,audienceData->eraseTimeY,EaseIn );
	audienceData->eraseTimeY++;
	audienceData->eraseTime++;


	//�x�����⊮�I���@�㉺���[�v
	if( audienceData->eraseTimeY >= audienceData->compTime/4 )
	{
		float buf = audienceData->stPos.y;
		audienceData->stPos.y = audienceData->pos.y;
		audienceData->edPos.y = buf;
		audienceData->eraseTimeY = 0;
	}

	audienceData->poly->pos = audienceData->pos;
	
	if( audienceData->pos.x <= m_rightLimit.x )
	{
		audienceData->col = D3DXCOLOR(0,0,1,1);
		audienceData->poly->col = audienceData->col;
	}


	//�ړ��I��
	if( audienceData->eraseTime >= audienceData->compTime && audienceData->eraseTimeY == 0 )
	{
		audienceData->col = D3DXCOLOR(0,0,1,1);

		//�J�n�ʒu�����݈ʒu��
		audienceData->stPos = audienceData->pos;
		//xz�̏I���ʒu�͂������Ō��߂�
		audienceData->state = AudienceManager::GetStateStayBlue();
		audienceData->state->Init( audienceData );

		audienceData->poly->col = audienceData->col;

	}
	else if(audienceData->eraseTime >= audienceData->compTime)
	{
		audienceData->eraseTime = audienceData->compTime;
	}
}

// EOF
