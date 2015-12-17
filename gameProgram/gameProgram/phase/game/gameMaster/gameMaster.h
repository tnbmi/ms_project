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
class CommandManager;
class TimeManager;
class FbxTexImport;
class Ggy2DAnimationManager;
class Polygon2D;

class GameMaster
{
public:
	GameMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,GameImport *import,FbxTexImport *fbxTexImport,Debugproc *proc,PadXManager* padXMaster );
	virtual ~GameMaster(void);

	static bool Create(GameMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						GameImport* import,FbxTexImport *fbxTexImport,Debugproc* debugproc,PadXManager* padXManager);
	bool Initialize(void);
	void Finalize(void);
	bool Update(void);

	int redTeamScore(){return m_redTeamScore;}
	int blueTeamScore(){return m_blueTeamScore;}

	//�`�[���X�R�A���Z
	void AddTeamScore( const int addRedTeamVal,const int addBlueTeamVal );
	void DetermineTeamScore();
private:

	enum NEBUTAANIMATION
	{
		NANIM_WAIT,
		NANIM_ACOMUP,
		NANIM_BCOMUP,
		NANIM_ACOMR,
		NANIM_BCOMR,
		NANIM_ACOMDOWN,
		NANIM_BCOMDOWN,
		NANIM_ACOML,
		NANIM_BCOML,
		NANIM_SAME1,
		NANIM_SAME2,
		NANIM_SAME3,
		NANIM_WIN,
		NANIM_LOSE,
		Max
	};

	struct NEBTAANIMATIONFRAME
	{
		int stFrame;
		int edFrame;
		int polyGgyAnimIdx;
	};

	struct CUTIN
	{
		Polygon2D *pol;
		int time;
		int addVal;
		D3DXVECTOR3 stPos;
		D3DXVECTOR3 edPos;
		D3DXVECTOR3 stBufPos;
		D3DXVECTOR3 edBufPos;
	};

	//�J�b�g�C��
	static const int _cutInFrame = 20;
	CUTIN m_redTeamCutIn;
	CUTIN m_blueTeamCutIn;

	void SelectAnimation( const int judge,Player *player,Ggy2DAnimationManager *ggy,CUTIN *cutIn );
	void UpdateCutIn();

	NEBTAANIMATIONFRAME m_nebAnim[Max];

	static const int _ScoreMax = 500;//���X�R�A

	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	GameImport*	 m_import;
	Debugproc*		 m_debugProc;
	PadXManager*	 m_padXManager;
	FbxTexImport*	 m_fbxTexImport;
	//�����[
	Ggy2DAnimationManager *m_blueGgyAnim;
	Ggy2DAnimationManager *m_redGgyAnim;



	//�ϋq����
	AudienceManager *m_audienceManager;
	//�G�t�F�N�g����
	EffectManager  *m_effectManager;

	//�v���C���[
	Player *m_redTeam;
	Player *m_blueTeam;

	// �R�}���h�}�l�[�W��
	CommandManager*	m_command_manager;
	// �^�C�}�[�}�l�[�W��
	TimeManager*	m_time_manager;

	

	int m_totalScore;
	int m_redTeamAddVal;
	int m_blueTeamAddVal;
	int m_redTeamScore;
	int m_blueTeamScore;
};

//=============================================================================
#endif

// EOF