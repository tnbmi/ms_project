//*****************************************************************************
//
// �R�}���h�`�F�b�N�����}�l�[�W�� [commandmanager.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_COMMAND_MANAGER_H_
#define MY_COMMAND_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Commandteam;
class PadXManager;
class Debugproc;

class Commandmanager
{
public:
	Commandmanager(void);
	virtual ~Commandmanager(void);

	static bool Create(Commandmanager** outPointer, PadXManager* padXManager, Debugproc* debugproc);
	bool Initialize(PadXManager* padXManager, Debugproc* debugproc);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	Commandteam*	m_team[2];
	int*			m_command_list;
	int				m_progress;
	Debugproc*		m_debugproc;
};

//=============================================================================
#endif

// EOF
