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
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"

#include "..\..\..\audienceManager\audienceManager.h"
#include "..\..\..\effectManager\effectManager.h"

#include "..\..\..\object\player\player.h"


//=============================================================================
// �R���X�g���N�^
//=============================================================================
GameMaster::GameMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,GameImport *import,Debugproc *proc,PadXManager* padXMaster )
{
	//
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	//----------------------------
	// �����o�[������
	//----------------------------
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
	//�Q�[���}�X�^�[����
	GameMaster* pointer = new GameMaster( device,objectList,updateList,drawList,import,debugproc,padXManager );
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

	//�R�}���h�}�l�[�W��

	//�ϋq���䐶��
	AudienceManager *audience;
	if(!AudienceManager::Create( &audience,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,
							 "./resources/texture/boy.png",_ScoreMax,D3DXVECTOR3( 100,50,-1000),D3DXVECTOR3(1000,200,-700 ),D3DXVECTOR3(  -1000,50,-1000),D3DXVECTOR3( -100,200,-700 ) ))
		return false;

	//�G�t�F�N�g�}�l�[�W��
	EffectManager *ef;
	if(!EffectManager::Create( &ef,m_device,m_objectList,m_updateList,m_drawListManager,5000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) ))
		return false;

	//�v���C���[����
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebblue.bin");
	Player::Create( &redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebred.bin");

	redTeam->StartAnimationSecondChild( 1,390,true );
	blueTeam->StartAnimationSecondChild( 1,390,true );

	m_audienceManager = audience;
	m_effectManager   = ef;
	m_blueTeam = blueTeam;
	m_redTeam  = redTeam;

	//----------------------------
	// �z�u
	//----------------------------
	m_redTeamAddVal = 0;
	m_blueTeamAddVal=0;
	m_totalScore = _ScoreMax;
	m_redTeamScore = _ScoreMax/2;
	m_blueTeamScore=_ScoreMax/2;
	m_redTeam->rot( D3DXVECTOR3(0,-3*D3DX_PI /4,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,3*D3DX_PI/4,0 ) );
	m_redTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );
	m_redTeam->secondOffsetPos( D3DXVECTOR3(0,0,0));
	m_blueTeam->offsetPos( D3DXVECTOR3(0,0,0 ) );

	m_redTeam->Move( D3DXVECTOR3(700,0,0),D3DXVECTOR3(700,0,0),300 );
	m_blueTeam->Move( D3DXVECTOR3(-700,0,0),D3DXVECTOR3(-700,0,0),300 );

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void GameMaster::Finalize(void)
{

	//�v���C���[�폜
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );

	SafeFinalizeDelete( m_audienceManager );
	SafeFinalizeDelete( m_effectManager );
}

//=============================================================================
// �X�V
//=============================================================================
void GameMaster::Update(void)
{

	//�v���C���[�X�V
	m_redTeam->Update();
	m_blueTeam->Update();

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