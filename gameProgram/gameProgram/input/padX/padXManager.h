//*****************************************************************************
//
// XInput�Q�[���p�b�h���͊Ǘ� [padXManager.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_PAD_X_MANGER_H
#define _MY_PAD_X_MANGER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PAD_MAX (4)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;
class PadX;

class PadXManager
{
public:
	PadXManager(void);
	~PadXManager(void);

	static bool Create(PadXManager** outPointer);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);

	void debugproc(Debugproc* debugproc);

	PadX* pad(int no) {return m_pad[no];}

	bool InputChkTrigger(void);
	bool InputChkTrigger(WORD wButton);
	bool InputChkPress(void);
	bool InputChkPress(WORD wButton);

private:
	PadX* m_pad[PAD_MAX];
};

//=============================================================================
#endif

// [EOF]
