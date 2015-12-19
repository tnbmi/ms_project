//*****************************************************************************
//
// �ւ���XInput�Q�[���p�b�h���� [dummyPadX.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_DUMMY_PAD_X_H
#define _MY_DUMMY_PAD_X_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\padX\padX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;
class DummyDataLoad;

class DummyPadX : public PadX
{
public:
	DummyPadX(void);
	~DummyPadX(void);

	static bool Create(PadX** outPointer, int no, int patternMax);
	bool Initialize(int no, int patternMax);
	void Finalize(void);
	void Update(void);

private:
	DummyDataLoad* m_dummyDataLoad;
	unsigned int* m_commandList;
};

//=============================================================================
#endif

// [EOF]
