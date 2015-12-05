//*****************************************************************************
//
// コマンドチェック処理マネージャ [commandmanager.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_COMMAND_MANAGER_H_
#define MY_COMMAND_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CommandDataLoad;
class CommandTeam;
class PadXManager;
class ObjectList;
class UpdateList;
class DrawListManager;
class Debugproc;
class GameImport;
class Polygon2D;

class CommandManager
{
public:
	CommandManager(void);
	~CommandManager(void);

	typedef enum{
		DRAW = 0,
		TEAM0_WIN,
		TEAM1_WIN
	}GAME_STATE;

	static bool Create(CommandManager** outPointer,
					   PadXManager* padXManager,
					   Debugproc* debugproc,
					   ObjectList* objList,
					   UpdateList* updList,
					   DrawListManager* drwList,
					   LPDIRECT3DDEVICE9 device,
					   GameImport* import);
	bool Initialize(PadXManager* padXManager,
					Debugproc* debugproc,
					ObjectList* objList,
					UpdateList* updList,
					DrawListManager* drwList,
					LPDIRECT3DDEVICE9 device,
					GameImport* import);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	GAME_STATE GetState(void){return m_game_state;}

private:
	CommandDataLoad* m_commandDataLoad;

	CommandTeam*	m_team[2];
	int*			m_command_list;
	int				m_progress;
	GAME_STATE		m_game_state;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	GameImport*	m_import;

	Polygon2D*	m_test_gage[10];
	void GageUpd(void);

	Debugproc*		m_debugproc;
};

//=============================================================================
#endif

// EOF
