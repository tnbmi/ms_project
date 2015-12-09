//*****************************************************************************
//
// �Q�[���}�X�^�[ [gameMaster.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef GAME_MASTER_H_
#define GAME_MASTER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\main\main.h"
#include "..\..\..\objectBase\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class AudienceManager;
class ObjectList;
class UpdateList;
class DrawListManager;
class EffectManager;
class GameImport;
class Debugproc;
class PadXManager;
class Player;



class GameMaster
{
public:
	GameMaster(AudienceManager *audienceManager,EffectManager *effectManager,Player *redTeam,Player *blueTeam);
	virtual ~GameMaster(void);

	static bool Create(GameMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						GameImport* import,Debugproc* debugproc,PadXManager* padXManager);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);

	//�e�X�g�p�Z�b�^�[�Q�b�^�[�Q
	//int totalScore(){ return m_totalScore; }
	//void totalScore( const int score ){ m_totalScore = score; }
	//int redTeamAddVal(){ return m_redTeamAddVal; }
	//int blueTeamAddVal(){ return m_blueTeamAddVal; }
	//void redTeamAddVal( const int Val ){ m_redTeamAddVal = Val; }
	///void blueTeamAddVal( const int Val ){ m_blueTeamAddVal = Val; }
	int redTeamScore(){return m_redTeamScore;}
	int blueTeamScore(){return m_blueTeamScore;}

	//�`�[���X�R�A���Z
	void AddTeamScore( const int addRedTeamVal,const int addBlueTeamVal );
	void DetermineTeamScore();
private:
	static const int _ScoreMax = 500;//���X�R�A

	//�ϋq����
	AudienceManager *m_audienceManager;

	//�G�t�F�N�g����
	EffectManager  *m_effectManager;

	//�v���C���[
	Player *m_redTeam;
	Player *m_blueTeam;

	int m_totalScore;
	int m_redTeamAddVal;
	int m_blueTeamAddVal;
	int m_redTeamScore;
	int m_blueTeamScore;
};

//=============================================================================
#endif

// EOF