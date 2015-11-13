//*****************************************************************************
//
// チームのコマンド管理 [commandteam.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_COMMANDTEAM_H_
#define MY_COMMANDTEAM_H_
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
	void Update(void);
	void Draw(void);

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}
	void playerset(PadX* p1, PadX* p2) {m_pad[0] = p1; m_pad[1] = p2;}

private:
	int				m_command_list[6];
	PadX*			m_pad[2];
	Debugproc*		m_debugproc;
	int				m_command_count;
	int				m_time_penalty;
};

//=============================================================================
#endif

// EOF
