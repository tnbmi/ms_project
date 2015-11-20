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
class Commandteam;
class PadXManager;
class ObjectList;
class UpdateList;
class DrawListManager;
class Debugproc;
class GameImport;

class Commandmanager
{
public:
	Commandmanager(void);
	virtual ~Commandmanager(void);

	static bool Create(Commandmanager** outPointer,
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

private:
	Commandteam*	m_team[2];
	int*			m_command_list;
	int				m_progress;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	Debugproc*		m_debugproc;
};

//=============================================================================
#endif

// EOF
