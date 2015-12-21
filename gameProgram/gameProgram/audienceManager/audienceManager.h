//*****************************************************************************
//
// �ϋq���� [audioenceManager.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef AUDIENCE_MANAGER_H_
#define AUDIENCE_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"
#include "..\objectBase\objectBase.h"
#include "..\objectBase\instancingBillboard\instancingBillboard.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DrawListManager;
class UpdateList;
class AudienceState;
class AudienceStateStayRedTeam;
class AudienceStateStayBlueTeam;
class AudienceStateMoveRedTeam;
class AudienceStateMoveBlueTeam;


//�r���{�[�h�N���X�̍\���̂��g���̂ŃC���N���[�h
class AudienceManager
{
public:

	enum TEAM
	{
		TEAM_RED = -1,
		TEAM_BLUE = 1,
		_TeamMax
	};

	struct AUDIENCEDATA
	{
		InstancingBillboard::POLYGONDATA *poly;//�\���p���
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scl;
		D3DXCOLOR   col;//�w�c�J���[ Red or Blue

		//�`�[��
		TEAM team;
		AudienceState *state;

		//�⊮�p
		D3DXVECTOR3 stPos;
		D3DXVECTOR3 edPos;
		float compTime;//�⊮�ɕK�v�Ȏ���
		float eraseTime;//���ݎ���
		float eraseTimeY;//Y�W�����v�⊮�p���ݎ���
		float addY;
	};

	AudienceManager( InstancingBillboard *insBill,const int scoreMax );
	virtual ~AudienceManager(void);

	static bool Create(AudienceManager** outPointer,LPDIRECT3DDEVICE9 device, ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						int priority  , ObjectBase::OBJECT_TYPE type,const char* textureLoadPath,const int scoreMax,
						const D3DXVECTOR3 &leftPosLimRed,const D3DXVECTOR3 &rightPosLimRed,
						const D3DXVECTOR3 &leftPosLimBlue,const D3DXVECTOR3 &rightPosLimBlue);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	bool MoveRedTeamFromBlueTeam( const int moveVal );
	bool MoveBlueTeamFromRedTeam( const int moveVal );
	static AudienceStateStayRedTeam *GetStateStayRed(){ return m_stateStayRed; }
	static AudienceStateStayBlueTeam *GetStateStayBlue(){ return m_stateStayBlue; }
	static AudienceStateMoveRedTeam *GetStateMoveRed(){ return m_stateRedMove; }
	static AudienceStateMoveBlueTeam *GetStateMoveBlue(){ return m_stateBlueMove; }

	void SetRedTeamPosLimit  ( const D3DXVECTOR3 &leftLim,const D3DXVECTOR3 &rightLim);
	void SetBlueTeamPosLimit ( const D3DXVECTOR3 &leftLim,const D3DXVECTOR3 &rightLim);

private:

	//�ړ��ł��Ȃ����������ړ��ł���ꍇ��������
	void ReduceMoveBuff();

	static int m_refCnt;	
	static AudienceStateStayRedTeam *m_stateStayRed;
	static AudienceStateStayBlueTeam*m_stateStayBlue;
	static AudienceStateMoveRedTeam *m_stateRedMove;
	static AudienceStateMoveBlueTeam *m_stateBlueMove;

	static const int _audienceMax = 100;//�ϋq�̍ő吔
	InstancingBillboard *m_insBill;//�C���X�^���V���O���C��

	AUDIENCEDATA *m_audienceArray;
	int m_audienceSum;
	int m_redTeamMoveBuff;
	int m_blueTeamMoveBuff;
};

//�X�e�[�g
class AudienceState
{
	protected:
		D3DXVECTOR3 m_leftLimit;
		D3DXVECTOR3 m_rightLimit;
	public:
		AudienceState(){}
		virtual ~AudienceState(){}
		virtual void Init( AudienceManager::AUDIENCEDATA *audienceData ) = 0;
		virtual void Update( AudienceManager::AUDIENCEDATA *audienceData ) = 0;
		virtual bool isMove() = 0;
		virtual void leftLimit ( const D3DXVECTOR3 &Lim ){ m_leftLimit = Lim; }
		virtual void rightLimit( const D3DXVECTOR3 &Lim ){ m_rightLimit= Lim; }
};

//�؍�
class AudienceStateStayRedTeam : public AudienceState
{
	public:
		AudienceStateStayRedTeam(){}
		~AudienceStateStayRedTeam(){}
		void Init( AudienceManager::AUDIENCEDATA *audienceData);
		void Update( AudienceManager::AUDIENCEDATA *audienceData );
		bool isMove(){ return true; }
};

class AudienceStateStayBlueTeam : public AudienceState
{
	public:
		AudienceStateStayBlueTeam(){}
		~AudienceStateStayBlueTeam(){}
		void Init( AudienceManager::AUDIENCEDATA *audienceData);
		void Update( AudienceManager::AUDIENCEDATA *audienceData );
		bool isMove(){ return true; }
};

//�`�[���ړ���
class AudienceStateMoveRedTeam : public AudienceState
{
	public:
		AudienceStateMoveRedTeam(){}
		~AudienceStateMoveRedTeam(){}
		void Init( AudienceManager::AUDIENCEDATA *audienceData );
		void Update( AudienceManager::AUDIENCEDATA *audienceData );
		bool isMove(){ return false;}
};

class AudienceStateMoveBlueTeam : public AudienceState
{
	public:
		AudienceStateMoveBlueTeam(){}
		~AudienceStateMoveBlueTeam(){}
		void Init( AudienceManager::AUDIENCEDATA *audienceData );
		void Update( AudienceManager::AUDIENCEDATA *audienceData );
		bool isMove(){ return false;}
};

//=============================================================================
#endif

// EOF