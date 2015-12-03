//*****************************************************************************
//
// チームのコマンド管理 [commandteam.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_COMMAND_TEAM_H_
#define MY_COMMAND_TEAM_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;
class PadX;
class ObjectList;
class UpdateList;
class DrawListManager;
class GameImport;
class Polygon2D;

class CommandTeam
{
public:
	CommandTeam(void);
	~CommandTeam(void);

	typedef enum{
		COLOR_BLUE = 0,
		COLOR_RED
	}TEAM_COLOR;

	static bool Create(CommandTeam** outPointer,
					   ObjectList* objList,
					   UpdateList* updList,
					   DrawListManager* drwList,
					   LPDIRECT3DDEVICE9 device,
					   GameImport* import,
					   D3DXVECTOR3 pos,
					   TEAM_COLOR team_color);
	bool Initialize(ObjectList* objList,
					UpdateList* updList,
					DrawListManager* drwList,
					LPDIRECT3DDEVICE9 device,
					GameImport* import,
					D3DXVECTOR3 pos,
					TEAM_COLOR team_color);
	void Finalize(void);
	bool Update(void);
	void Draw(void);

	void SetPlayer(PadX* p1, PadX* p2){m_pad[0] = p1; m_pad[1] = p2;}
	void SetFragLose(bool flag);
	void SetCommand(int* command);

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

private:
	int*		m_command;
	PadX*		m_pad[2];
	TEAM_COLOR	m_team_color;
	int			m_command_count;
	int			m_command_long;
	int			m_time_penalty;
	bool		m_flag_lose;
	Polygon2D*	m_command_poly[6];
	Polygon2D*	m_back_poly[2];
	Debugproc*	m_debugproc;

	GameImport*	m_import;
	LPDIRECT3DDEVICE9	m_device;

	ObjectList*			m_objectList;
	UpdateList*			m_updateList;
	DrawListManager*	m_drawListManager;

	D3DXVECTOR3			m_polygon_pos;

	bool	InitObject(void);
	void	SetSuccess(void);
	void	SetPenalty(void);
	void	StateReset(void);
};

//=============================================================================
#endif

// EOF
