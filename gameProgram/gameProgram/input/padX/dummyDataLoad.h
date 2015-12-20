//*****************************************************************************
//
// �_�~�[���̓f�[�^�̃��[�h [dummyDataLoad.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_COMMAND_DATA_LOAD_H_
#define MY_COMMAND_DATA_LOAD_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DummyDataLoad
{
public:
	DummyDataLoad(int patternMax);
	~DummyDataLoad(void);

	static bool Create(DummyDataLoad** outPointer, int patternMax, unsigned int* commandData);
	bool Initialize(unsigned int* commandData);
	void Finalize(void);

	unsigned int* dummyData(void){return m_dummyData;}

private:
	int m_patternMax;

	unsigned int* m_commandData;
};

//=============================================================================
#endif

// EOF
