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
#include "commandteam\commandteam.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define COMMAND_MAX (10)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CommandDataLoad;
class PadXManager;
class ObjectList;
class UpdateList;
class DrawListManager;
class Debugproc;
class Import;
class Polygon2D;

class CommandManager
{
public:
	CommandManager(void);
	~CommandManager(void);

	typedef struct{
		int	score[2];
		CommandTeam::COMMAND_STATE state[2];
	}COM_MANA_RTN;

	static bool Create(CommandManager** outPointer,
					   PadXManager* padXManager,
					   Debugproc* debugproc,
					   ObjectList* objList,
					   UpdateList* updList,
					   DrawListManager* drwList,
					   LPDIRECT3DDEVICE9 device,
					   Import* import,
					   bool demoFlg = false);
	bool Initialize(PadXManager* padXManager,
					Debugproc* debugproc,
					ObjectList* objList,
					UpdateList* updList,
					DrawListManager* drwList,
					LPDIRECT3DDEVICE9 device,
					Import* import,
					bool demoFlg);
	void Finalize(void);
	COM_MANA_RTN Update(void);
	void Draw(void);

	void SetClimaxFlag(bool b){m_climax_flag = b;}

private:
	CommandDataLoad* m_commandDataLoad;

	CommandTeam*	m_team[2];
	int				m_command_prev[2];
	unsigned int*	m_command_list[2];
	PadX*			m_pad[4];

	Polygon2D*		m_ui_polygon;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	Import*	m_import;

	Debugproc*		m_debugproc;

	bool m_demoFlg;
	bool m_climax_flag;
};

//=============================================================================
#endif

// EOF
