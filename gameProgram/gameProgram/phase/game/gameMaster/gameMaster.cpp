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

#include "..\..\..\commandmanager\commandmanager.h"
#include "..\..\..\timemanager\timeManager.h"

#include "..\..\..\object\player\player.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _time_max		= 5460;
const D3DXVECTOR3 _effect_pos[2] = {D3DXVECTOR3(-900.0f, 400.0f, 0.0f),D3DXVECTOR3(900.0f, 400.0f, 0.0f)};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
GameMaster::GameMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,GameImport *import,FbxTexImport *fbxTexImport,Debugproc *proc,PadXManager* padXMaster )
{
	//
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
	m_fbxTexImport = fbxTexImport;
	m_command_manager	= nullptr;
	m_time_manager		= nullptr;
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
						GameImport* import,FbxTexImport *fbxTexImport,Debugproc* debugproc,PadXManager* padXManager)
{
	//�Q�[���}�X�^�[����
	GameMaster* pointer = new GameMaster( device,objectList,updateList,drawList,import,fbxTexImport,debugproc,padXManager );
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
	CommandManager::Create(&m_command_manager, m_padXManager, m_debugProc, m_objectList, m_updateList, m_drawListManager, m_device, m_import);
	// �^�C���}�l�[�W������
	TimeManager::Create(&m_time_manager, m_objectList, m_updateList, m_drawListManager, m_device, m_import, _time_max);

	//�ϋq���䐶��
	AudienceManager *audience;
	if(!AudienceManager::Create( &audience,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,
							 "./resources/texture/boy.png",_ScoreMax,D3DXVECTOR3( 100.0f,50.0f,-1000.0f),D3DXVECTOR3(1000.0f,200.0f,-700.0f ),D3DXVECTOR3(  -1000.0f,50.0f,-1000.0f),D3DXVECTOR3( -100.0f,200.0f,-700.0f ) ))
		return false;

	//�G�t�F�N�g�}�l�[�W��
	EffectManager *ef;
	if(!EffectManager::Create( &ef,m_device,m_objectList,m_updateList,m_drawListManager,5000,"./resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) ))
		return false;

	//�v���C���[����
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebta_blue.bin",m_fbxTexImport);
	Player::Create( &redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"./resources/fbxModel/daisya.bin","./resources/fbxModel/ground.bin","./resources/fbxModel/nebta_red.bin",m_fbxTexImport);

	redTeam->StartAnimationSecondChild( 1,570,true );
	blueTeam->StartAnimationSecondChild( 1,570,true );

	redTeam->StartAnimationChild( 1,60,true );
	blueTeam->StartAnimationChild( 1,60,true );

	redTeam->StartAnimationParent( 1,60,true );
	blueTeam->StartAnimationParent( 1,60,true );

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

	m_redTeam->Move( D3DXVECTOR3(700.0f,0.0f,0.0f),D3DXVECTOR3(700.0f,0.0f,0.0f),300.0f );
	m_blueTeam->Move( D3DXVECTOR3(-700.0f,0.0f,0.0f),D3DXVECTOR3(-700.0f,0.0f,0.0f),300.0f );

	m_effectManager->LoadEffectData( "./resources/effect/FireWorks.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks2.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/FireWorks3.OEF" );
	m_effectManager->LoadEffectData( "./resources/effect/Ene.OEF" );
	m_effectManager->SetOption( InstancingBillboard::OPTION( true,false,false ));

	//�A�j���[�V�����f�[�^����Ă��
	m_nebAnim[NANIM_WAIT].stFrame = 1;
	m_nebAnim[NANIM_WAIT].edFrame = 30;

	m_nebAnim[NANIM_ACOMUP].stFrame = 31;
	m_nebAnim[NANIM_ACOMUP].edFrame = 60;

	m_nebAnim[NANIM_BCOMUP].stFrame = 61;
	m_nebAnim[NANIM_BCOMUP].edFrame = 90;

	m_nebAnim[NANIM_ACOMR].stFrame = 91;
	m_nebAnim[NANIM_ACOMR].edFrame = 120;

	m_nebAnim[NANIM_BCOMR].stFrame = 121;
	m_nebAnim[NANIM_BCOMR].edFrame = 150;

	m_nebAnim[NANIM_ACOMDOWN].stFrame = 151;
	m_nebAnim[NANIM_ACOMDOWN].edFrame = 180;

	m_nebAnim[NANIM_BCOMDOWN].stFrame = 181;
	m_nebAnim[NANIM_BCOMDOWN].edFrame = 210;

	m_nebAnim[NANIM_ACOML].stFrame = 211;
	m_nebAnim[NANIM_ACOML].edFrame = 240;

	m_nebAnim[NANIM_BCOML].stFrame = 241;
	m_nebAnim[NANIM_BCOML].edFrame = 270;

	m_nebAnim[NANIM_SAME1].stFrame = 271;
	m_nebAnim[NANIM_SAME1].edFrame = 330;

	m_nebAnim[NANIM_SAME2].stFrame = 331;
	m_nebAnim[NANIM_SAME2].edFrame = 390;

	m_nebAnim[NANIM_SAME3].stFrame = 391;
	m_nebAnim[NANIM_SAME3].edFrame = 450;

	m_nebAnim[NANIM_WIN].stFrame = 451;
	m_nebAnim[NANIM_WIN].edFrame = 510;

	m_nebAnim[NANIM_LOSE].stFrame = 511;
	m_nebAnim[NANIM_LOSE].edFrame = 570;

	redTeam->StartAnimationSecondChild( m_nebAnim[NANIM_SAME1].stFrame,m_nebAnim[NANIM_SAME1].edFrame,true );
	blueTeam->StartAnimationSecondChild( 1,570,true );

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void GameMaster::Finalize(void)
{
	// �R�}���h
	SafeFinalizeDelete(m_command_manager);
	// �^�C��
	SafeFinalizeDelete(m_time_manager);

	//�v���C���[�폜
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );

	SafeFinalizeDelete( m_audienceManager );
	SafeFinalizeDelete( m_effectManager );
}

//=============================================================================
// �X�V
//=============================================================================
bool GameMaster::Update(void)
{
	// �G�t�F�N�g�}�l�[�W��
	m_effectManager->Update();

	//�v���C���[�X�V
	m_redTeam->Update();
	m_blueTeam->Update();

	// �R�}���h�}�l�[�W��
	CommandManager::COM_MANA_RTN get = {0,0};
	get = m_command_manager->Update();
	AddTeamScore(get.score[1], get.score[0]);
	for(int i = 0; i < 2; i++)
	{
		if(get.score[i] != 0)
		{
			m_effectManager->AddEffectFromDataBase( i, _effect_pos[i] );
		}
	}

	//�X�R�A�m��
	DetermineTeamScore();

	//�ړ��l��`����
	m_audienceManager->MoveBlueTeamFromRedTeam( m_blueTeamAddVal );
	m_audienceManager->MoveRedTeamFromBlueTeam( m_redTeamAddVal );

	//�ϋq�X�V
	m_audienceManager->Update();

	// �^�C���}�l�[�W��
	if(m_time_manager->Update())
		return true;

	m_blueTeamAddVal = 0;
	m_redTeamAddVal	 = 0;

	return false;
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