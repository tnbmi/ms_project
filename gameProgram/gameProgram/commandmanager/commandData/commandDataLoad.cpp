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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CommandDataLoad::CommandDataLoad(int patternMax)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_patternMax = patternMax;
	int size	 = patternMax * 10;

	m_commandDataA = new unsigned int[size];
	m_commandDataB = new unsigned int[size];

	for(int cnt = 0; cnt < size; ++cnt)
	{
		m_commandDataA[cnt] = 0;
		m_commandDataB[cnt] = 0;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CommandDataLoad::~CommandDataLoad(void)
{
	SafeDeleteArray(m_commandDataA);
	SafeDeleteArray(m_commandDataB);
}

//=============================================================================
// ����
//=============================================================================
bool CommandDataLoad::Create(CommandDataLoad** outPointer, const char* filePath, int patternMax)
{
	CommandDataLoad* pointer = new CommandDataLoad(patternMax);
	if(!pointer->Initialize(filePath))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool CommandDataLoad::Initialize(const char* filePath)
{
	FILE*	file;
	char	c;
	int		offsetA = 0;
	int		offsetB = 0;

	//----------------------------
	// �t�@�C���I�[�v��
	//----------------------------
	fopen_s(&file, filePath, "r");

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
						&m_commandDataA[offsetA],
						&m_commandDataA[offsetA + 1],
						&m_commandDataA[offsetA + 2],
						&m_commandDataA[offsetA + 3],
						&m_commandDataA[offsetA + 4],
						&m_commandDataA[offsetA + 5],
						&m_commandDataA[offsetA + 6],
						&m_commandDataA[offsetA + 7],
						&m_commandDataA[offsetA + 8],
						&m_commandDataA[offsetA + 9]);
				offsetA += 10;
			}
			else if(c == 'b')
			{// �v���C���[B
				fscanf_s(file,
						" %d %d %d %d %d %d %d %d %d %d",
						&m_commandDataB[offsetB],
						&m_commandDataB[offsetB + 1],
						&m_commandDataB[offsetB + 2],
						&m_commandDataB[offsetB + 3],
						&m_commandDataB[offsetB + 4],
						&m_commandDataB[offsetB + 5],
						&m_commandDataB[offsetB + 6],
						&m_commandDataB[offsetB + 7],
						&m_commandDataB[offsetB + 8],
						&m_commandDataB[offsetB + 9]);
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
