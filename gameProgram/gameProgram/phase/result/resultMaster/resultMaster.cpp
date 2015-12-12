//*****************************************************************************
//
// ���U���g�V�[���Ǘ� [resultMaster.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "resultMaster.h"
#include "..\..\..\common\safe.h"

#include "..\..\..\list\objectList\objectList.h"
#include "..\..\..\list\updateList\updateList.h"
#include "..\..\..\list\drawList\drawListManager.h"
#include "..\..\..\score\score.h"
#include "..\..\..\effectManager\effectManager.h"
#include "..\..\..\object\player\player.h"
#include "..\..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\..\common\complement\complement.h"


//=============================================================================
// �R���X�g���N�^
//=============================================================================
ResultMaster::ResultMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,ResultImport *import,Debugproc *proc,PadXManager* padXMaster )
{
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
	m_import = import;
	m_debugProc = proc;
	m_padXManager = padXMaster;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ResultMaster::~ResultMaster(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool ResultMaster::Create(ResultMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						ResultImport* import,Debugproc* debugproc,PadXManager* padXManager)
{

	ResultMaster* pointer = new ResultMaster( device,objectList,updateList,drawList,import,debugproc,padXManager );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool ResultMaster::Initialize(void)
{
	//�X�R�A����
	Score *redTeamScore;
	Score *blueTeamScore;
	Score::Create( &redTeamScore,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_2D,m_import );
	Score::Create( &blueTeamScore,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_2D,m_import );

	m_redTeamScore = redTeamScore;
	m_blueTeamScore= blueTeamScore;

	//�˂Ԃ�����
	Player *redTeam;
	Player *blueTeam;
	Player::Create( &blueTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"../resources/fbxModel/daisya.bin","../resources/fbxModel/ground.bin","../resources/fbxModel/robo.bin");
	Player::Create( &redTeam,m_device,m_objectList,m_updateList,m_drawListManager,0,ObjectBase::TYPE_3D,"../resources/fbxModel/daisya.bin","../resources/fbxModel/ground.bin","../resources/fbxModel/robo.bin");

	m_redTeam = redTeam;
	m_blueTeam= blueTeam;

	//����������񐶐�
	FbxModel *redGgy;
	FbxModel *blueGgy;
	FbxModel::Create( &redGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/gg_red.bin" );
	FbxModel::Create( &blueGgy,m_device,m_objectList,0,ObjectBase::TYPE_3D,"../resources/fbxModel/gg_blue.bin" );

	m_redGgy = redGgy;
	m_blueGgy= blueGgy;

	m_updateList->Link( redGgy );
	m_drawListManager->Link( redGgy,0,Shader::PAT_FBX );
	m_updateList->Link( blueGgy );
	m_drawListManager->Link( blueGgy,0,Shader::PAT_FBX );

	//�G�t�F�N�g�}�l�[�W������
	EffectManager *effectManager;
	EffectManager::Create( &effectManager,m_device,m_objectList,m_updateList,m_drawListManager,10000,"../resources/texture/effect.jpg",D3DXVECTOR2(1,1),D3DXVECTOR2(1,1) );

	m_effectManager = effectManager;

	//----------------------------
	//�����ʒu
	//----------------------------
	m_redTeamScore->pos( D3DXVECTOR3( 640 +320,280,0 ) );
	m_blueTeamScore->pos( D3DXVECTOR3( 320,280,0 ) );
	m_redTeamScore->StartRandView(200);
	m_redTeamScore->score(1419);
	m_redTeamScore->col(D3DXCOLOR(1,0,0,1));
	
	m_blueTeamScore->StartRandView(200);
	m_blueTeamScore->score(1145);
	m_blueTeamScore->col(D3DXCOLOR(0,0,1,1));

	m_redTeam->rot( D3DXVECTOR3(0,PAI,0 ) );
	m_blueTeam->rot( D3DXVECTOR3(0,PAI,0 ) );
	m_redTeam->Move( D3DXVECTOR3(-500,0,0),D3DXVECTOR3(-500,0,0),300 );
	m_blueTeam->Move( D3DXVECTOR3(500,0,0),D3DXVECTOR3(500,0,0),300 );

	m_redGgy->pos( D3DXVECTOR3(100,0,-1300) );
	m_blueGgy->pos( D3DXVECTOR3(-100,0,-1300) );
	m_redGgy->rot( D3DXVECTOR3(0,PAI,0));
	m_blueGgy->rot( D3DXVECTOR3(0,PAI,0));
	
	m_effectManager->LoadEffectData( "../resources/effect/FireWorks.OEF" );
	m_effectManager->LoadEffectData( "../resources/effect/FireWorks2.OEF" );
	m_effectManager->LoadEffectData( "../resources/effect/FireWorks3.OEF" );
	m_effectManager->LoadEffectData( "../resources/effect/Ene.OEF" );
	m_effectManager->SetOption( InstancingBillboard::OPTION( true,false,false ));

	m_phase = PHASE_RESULTSTART;
	return true;
}

//=============================================================================
// �I��
//=============================================================================
void ResultMaster::Finalize(void)
{
	SafeFinalizeDelete( m_redTeamScore );
	SafeFinalizeDelete( m_blueTeamScore );
	SafeFinalizeDelete( m_redTeam );
	SafeFinalizeDelete( m_blueTeam );
	SafeFinalizeDelete( m_effectManager );
}

//=============================================================================
// �X�V
//=============================================================================
void ResultMaster::Update(void)
{
	switch( m_phase )
	{
	case PHASE_RESULTSTART:

		m_redTeamScore->StartRandView( 60 * 3 );
		m_blueTeamScore->StartRandView( 60 * 3 );
		m_phase = PHASE_ANNOUNCEMENT;
		break;

	case PHASE_ANNOUNCEMENT:

		if( !m_redTeamScore->isRandView() )
		{

			m_phase = PHASE_ANNOUNCEFINISH;

			m_redGgy->StartAnimation(31,90,false );
			m_blueGgy->StartAnimation(91,150,false );

		}

		m_fireTime = _shotFrame;
		break;

	case PHASE_ANNOUNCEFINISH:

		if( m_fireTime >= _shotFrame )
		{
			D3DXVECTOR3 pos;
			pos.y = 1300;
			pos.x = RandRange(-400,-800);
			pos.z = RandRange(700,500);

			m_effectManager->AddEffectFromDataBase( 0,pos );

			pos.x = RandRange(400,-400);
			pos.z = RandRange(700,500);

			m_effectManager->AddEffectFromDataBase( 1,pos);

			pos.x = RandRange(800,400);
			pos.z = RandRange(700,500);

			m_effectManager->AddEffectFromDataBase( 2,pos );

			m_effectManager->AddEffectFromDataBase( 3,D3DXVECTOR3(0,1300,800) );
			m_fireTime = 0;
		}

		m_fireTime++;

		break;
	}

	m_redTeamScore->Update();
	m_blueTeamScore->Update();
	m_redTeam->Update();
	m_blueTeam->Update();
	m_effectManager->Update();
}

//=============================================================================
// �`��
//=============================================================================
void ResultMaster::Draw(void)
{
}
