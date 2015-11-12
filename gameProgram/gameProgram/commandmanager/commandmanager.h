//*****************************************************************************
//
// コマンドチェック処理 [commandmanager.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_COMMANDMANAGER_H_
#define MY_COMMANDMANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Keyboard;
class Debugproc;

class Commandmanager
{
public:
	Commandmanager(void);
	virtual ~Commandmanager(void);

	static bool Create(Commandmanager** outPointer);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	void keyboard(Keyboard* keyboard) {m_keyboard = keyboard;}
	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

private:
	int			m_command_list[6];
	Keyboard*	m_keyboard;
	Debugproc*	m_debugproc;
	int			m_command_count;
	int			m_time_penalty;
};

//=============================================================================
#endif

// EOF
