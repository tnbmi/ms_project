//*****************************************************************************
//
// �R�}���h�f�[�^�̃��[�h [commandDataLoad.h]
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
class CommandDataLoad
{
public:
	CommandDataLoad(int patternMax);
	~CommandDataLoad(void);

	static bool Create(CommandDataLoad** outPointer, int patternMax);
	bool Initialize(void);
	void Finalize(void);

	void commandDataLoadA(unsigned int* outData){outData = m_commandDataLoadA;}
	void commandDataLoadB(unsigned int* outData){outData = m_commandDataLoadB;}

private:
	int m_patternMax;
	int m_size;

	unsigned int* m_commandDataLoadA;
	unsigned int* m_commandDataLoadB;
};

//=============================================================================
#endif

// EOF
