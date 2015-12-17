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

	typedef enum{
		STATE_UP = 0,
		STATE_DOWN,
		STATE_LEFT,
		STATE_RIGHT,
		STATE_DOUBLE,
		STATE_FAIL,
		STATE_NONE,
		STATE_WAIT
	}COMMAND_STATE;

	typedef struct{
		unsigned int num_data;
		Polygon2D* polygon_pointer;
		float pos_y;
		bool hit;
		COMMAND_STATE state;
		int anime_count;
	}COMMAND_DATA;

	typedef struct{
		int return_score;
		bool flag;
		COMMAND_STATE state;
	}COM_TEAM_RTN;

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
	COM_TEAM_RTN Update(void);
	void Draw(void);

	void SetPlayer(PadX* p1, PadX* p2){m_pad[0] = p1; m_pad[1] = p2;}
	void SetCommand(unsigned int* command, unsigned int* nextCommand, int player, float offset);
	void SetCommandNext(unsigned int* command, int player, float offset = 0.0f);

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

private:
	unsigned int*	m_command_pointer_Next[2];
	COMMAND_DATA	m_command_data[2][10];
	PadX*			m_pad[2];
	TEAM_COLOR		m_team_color;
	int				m_command_count;
	int				m_delete_count;
	int				m_same_count;
	Polygon2D*		m_command_poly[2][10];
	Polygon2D*		m_back_poly[2];
	Debugproc*		m_debugproc;
	float			m_speed;
	float			m_offset;

	GameImport*	m_import;
	LPDIRECT3DDEVICE9	m_device;

	ObjectList*			m_objectList;
	UpdateList*			m_updateList;
	DrawListManager*	m_drawListManager;

	D3DXVECTOR3			m_polygon_pos;

	bool	InitObject(void);
	void	SetPolygon(int player);
};

//=============================================================================
#endif

// EOF
