//*****************************************************************************
//
// �Q�[���}�X�^�[ [GameMaster.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "GameMaster.h"
#include "..\..\..\common\safe.h"
#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\drawList\drawListManager.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\audienceManager\audienceManager.h"
#include "..\..\..\effectManager\effectManager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
GameMaster::GameMaster(AudienceManager *audienceManager,EffectManager *effectManager)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_audienceManager = audienceManager;
	m_effectManager   = effectManager;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
GameMaster::~GameMaster(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool GameMaster::Create(GameMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						GameImport* import,Debugproc* debugproc,PadXManager* padXManager)
{
	//�R�}���h�}�l�[�W��

	//�ϋq���䐶��
	AudienceManager *audience;
	if(!AudienceManager::Create( &audience,device,objectList,updateList,drawList,0,ObjectBase::TYPE_3D,
							 "../resources/texture/boy.png",_ScoreMax,D3DXVECTOR3( 1000,0,-1000),D3DXVECTOR3(2000,40,0 ),D3DXVECTOR3(  -2000,0,-1000),D3DXVECTOR3( -1000,40,0 ) ))
		return false;

	//�G�t�F�N�g�}�l�[�W��
	EffectManager *ef;
	if(!EffectManager::Create( &ef,device,objectList,updateList,drawList,5000,"../resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) ))
		return false;

	GameMaster* pointer = new GameMaster( audience,ef );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool GameMaster::Initialize(void)
{
	//----------------------------
	// �R�����g
	//----------------------------
	m_redTeamAddVal = 0;
	m_blueTeamAddVal=0;
	m_totalScore = _ScoreMax;
	m_redTeamScore = _ScoreMax/2;
	m_blueTeamScore=_ScoreMax/2;
	return true;
}

//=============================================================================
// �I��
//=============================================================================
void GameMaster::Finalize(void)
{
	SafeFinalizeDelete( m_audienceManager );
	SafeFinalizeDelete( m_effectManager );
}

//=============================================================================
// �X�V
//=============================================================================
void GameMaster::Update(void)
{

	//�X�R�A�m��
	DetermineTeamScore();

	//�ړ��l��`����
	m_audienceManager->MoveBlueTeamFromRedTeam( m_blueTeamAddVal );
	m_audienceManager->MoveRedTeamFromBlueTeam( m_redTeamAddVal );

	//�ϋq�X�V
	m_audienceManager->Update();
}

//=============================================================================
//
//=============================================================================

void GameMaster::AddTeamScore( const int addRedTeamVal,const int addBlueTeamVal )
{
	int addRVal = addRedTeamVal - addBlueTeamVal;
	int addBVal = addBlueTeamVal- addRedTeamVal;

	m_redTeamAddVal += addRVal;
	m_blueTeamAddVal+= addBVal;
}

//=============================================================================
//
//=============================================================================

void GameMaster::DetermineTeamScore()
{
	int rOffset = 0;
	int bOffset = 0;
	int rScoreBuff = m_redTeamScore + m_redTeamAddVal;
	int bScoreBuff = m_blueTeamScore+ m_blueTeamAddVal;

	//�e�`�[�����X�R�A�����ĂȂ����`�F�b�N
	if( rScoreBuff < 0 )
	{
		rOffset = rScoreBuff;
	}
	else if( rScoreBuff > m_totalScore )
	{
		rOffset = rScoreBuff - m_totalScore;
	}

	if( bScoreBuff < 0 )
	{
		bOffset = bScoreBuff;
	}
	else if( bScoreBuff > m_totalScore )
	{
		bOffset = bScoreBuff - m_totalScore;
	}

	m_redTeamAddVal -= rOffset;
	m_blueTeamAddVal-= bOffset;

	m_redTeamScore += m_redTeamAddVal;
	m_blueTeamScore += m_blueTeamAddVal;
}


// EOF