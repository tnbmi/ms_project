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

class GameMaster
{
public:
	GameMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,GameImport *import,Debugproc *proc,PadXManager* padXMaster );
	virtual ~GameMaster(void);

	static bool Create(GameMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						GameImport* import,Debugproc* debugproc,PadXManager* padXManager);
	bool Initialize(void);
	void Finalize(void);
	bool Update(void);

	//テスト用セッターゲッター群
	//int totalScore(){ return m_totalScore; }
	//void totalScore( const int score ){ m_totalScore = score; }
	//int redTeamAddVal(){ return m_redTeamAddVal; }
	//int blueTeamAddVal(){ return m_blueTeamAddVal; }
	//void redTeamAddVal( const int Val ){ m_redTeamAddVal = Val; }
	///void blueTeamAddVal( const int Val ){ m_blueTeamAddVal = Val; }
	int redTeamScore(){return m_redTeamScore;}
	int blueTeamScore(){return m_blueTeamScore;}

	//チームスコア加算
	void AddTeamScore( const int addRedTeamVal,const int addBlueTeamVal );
	void DetermineTeamScore();
private:
	static const int _ScoreMax = 500;//総スコア

	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	GameImport*	 m_import;
	Debugproc*		 m_debugProc;
	PadXManager*	 m_padXManager;

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