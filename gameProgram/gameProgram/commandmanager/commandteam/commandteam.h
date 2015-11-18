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

class Commandteam
{
public:
	Commandteam(void);
	virtual ~Commandteam(void);

	static bool Create(Commandteam** outPointer);
	bool Initialize(void);
	void Finalize(void);
	bool Update(void);
	void Draw(void);

	void SetPlayer(PadX* p1, PadX* p2){m_pad[0] = p1; m_pad[1] = p2;}
	void SetFragLose(bool flag);
	void SetCommand(int* command){m_command = command;}
	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

private:
	int*			m_command;
	PadX*			m_pad[2];
	int				m_command_count;
	int				m_command_long;
	int				m_time_penalty;
	bool			m_flag_lose;
	Debugproc*		m_debugproc;
};

//=============================================================================
#endif

// EOF
