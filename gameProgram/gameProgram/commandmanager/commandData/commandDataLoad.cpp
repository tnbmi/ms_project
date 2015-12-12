//*****************************************************************************
//
// �R�}���h�f�[�^�̃��[�h [commandDataLoad.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandDataLoad.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char* _command_data = "./resources/command/comData.txt";

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CommandDataLoad::CommandDataLoad(int patternMax)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_patternMax = patternMax;
	m_size		 = patternMax * 10;

	m_commandDataLoadA = new unsigned int[m_size];
	m_commandDataLoadB = new unsigned int[m_size];

	for(int cnt = 0; cnt < m_size; ++cnt)
	{
		m_commandDataLoadA[cnt] = 0;
		m_commandDataLoadB[cnt] = 0;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CommandDataLoad::~CommandDataLoad(void)
{
	SafeDeleteArray(m_commandDataLoadA);
	SafeDeleteArray(m_commandDataLoadB);
}

//=============================================================================
// ����
//=============================================================================
bool CommandDataLoad::Create(CommandDataLoad** outPointer, int patternMax)
{
	CommandDataLoad* pointer = new CommandDataLoad(patternMax);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool CommandDataLoad::Initialize(void)
{
	FILE*	file;
	char	c;
	int		offsetA = 0;
	int		offsetB = 0;

	//----------------------------
	// �t�@�C���I�[�v��
	//----------------------------
	fopen_s(&file, _command_data, "r");

	if(file == nullptr)
		return false;

	//----------------------------
	// �R�}���h�f�[�^�ǂݍ���
	//----------------------------
	while((c = fgetc(file)) != EOF)
	{
		if(c == '/')
		{
			c = fgetc(file);
			if(c == 'a')
			{// �v���C���[A
				fscanf_s(file,
						" %d %d %d %d %d %d %d %d %d %d",
						&m_commandDataLoadA[offsetA],
						&m_commandDataLoadA[offsetA + 1],
						&m_commandDataLoadA[offsetA + 2],
						&m_commandDataLoadA[offsetA + 3],
						&m_commandDataLoadA[offsetA + 4],
						&m_commandDataLoadA[offsetA + 5],
						&m_commandDataLoadA[offsetA + 6],
						&m_commandDataLoadA[offsetA + 7],
						&m_commandDataLoadA[offsetA + 8],
						&m_commandDataLoadA[offsetA + 9]);
				offsetA += 10;
			}
			else if(c == 'b')
			{// �v���C���[B
				fscanf_s(file,
						" %d %d %d %d %d %d %d %d %d %d",
						&m_commandDataLoadB[offsetB],
						&m_commandDataLoadB[offsetB + 1],
						&m_commandDataLoadB[offsetB + 2],
						&m_commandDataLoadB[offsetB + 3],
						&m_commandDataLoadB[offsetB + 4],
						&m_commandDataLoadB[offsetB + 5],
						&m_commandDataLoadB[offsetB + 6],
						&m_commandDataLoadB[offsetB + 7],
						&m_commandDataLoadB[offsetB + 8],
						&m_commandDataLoadB[offsetB + 9]);
				offsetB += 10;
			}
		}
	}

	//----------------------------
	// �t�@�C���N���[�Y
	//----------------------------
	fclose(file);

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void CommandDataLoad::Finalize(void)
{
}

// EOF
