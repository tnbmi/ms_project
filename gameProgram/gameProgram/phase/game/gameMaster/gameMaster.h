//*****************************************************************************
//
// ゲームマスター [gameMaster.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef GAME_MASTER_H_
#define GAME_MASTER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\main\main.h"
#include "..\..\..\objectBase\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
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

	//チームスコア加算
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

	NEBTAANIMATIONFRAME m_nebAnim[Max];

	static const int _ScoreMax = 500;//総スコア

	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	GameImport*	 m_import;
	Debugproc*		 m_debugProc;
	PadXManager*	 m_padXManager;
	FbxTexImport*	 m_fbxTexImport;
	//じじー
	Ggy2DAnimationManager *m_blueGgyAnim;
	Ggy2DAnimationManager *m_redGgyAnim;


	//観客制御
	AudienceManager *m_audienceManager;
	//エフェクト制御
	EffectManager  *m_effectManager;

	//プレイヤー
	Player *m_redTeam;
	Player *m_blueTeam;

	// コマンドマネージャ
	CommandManager*	m_command_manager;
	// タイマーマネージャ
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