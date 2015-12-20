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

	static bool Create(PadX** outPointer, int no, int patternMax, unsigned int* commandList, int per);
	bool Initialize(int no, int patternMax, unsigned int* commandList, int per);
	void Finalize(void);
	void Update(void);

private:
	unsigned int* m_commandList;
	int m_per;
};

//=============================================================================
#endif

// [EOF]
