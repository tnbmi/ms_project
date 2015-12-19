//*****************************************************************************
//
// �_�~�[���̓f�[�^�̃��[�h [dummyDataLoad.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dummyDataLoad.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// �R���X�g���N�^
//=============================================================================
DummyDataLoad::DummyDataLoad(int patternMax)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_patternMax = patternMax;
	int size	 = patternMax * 10;

	m_dummyData = new unsigned int[size];

	for(int cnt = 0; cnt < size; ++cnt)
		m_dummyData[cnt] = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
DummyDataLoad::~DummyDataLoad(void)
{
	SafeDeleteArray(m_dummyData);
}

//=============================================================================
// ����
//=============================================================================
bool DummyDataLoad::Create(DummyDataLoad** outPointer, const char* filePath, int patternMax)
{
	DummyDataLoad* pointer = new DummyDataLoad(patternMax);
	if(!pointer->Initialize(filePath))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool DummyDataLoad::Initialize(const char* filePath)
{
	FILE*	file;
	char	c;
	int		offset = 0;

	//----------------------------
	// �t�@�C���I�[�v��
	//----------------------------
	fopen_s(&file, filePath, "r");

	if(file == nullptr)
		return false;

	//----------------------------
	// �_�~�[���̓f�[�^�ǂݍ���
	//----------------------------
	while((c = fgetc(file)) != EOF)
	{
		if(c == '/')
		{
			c = fgetc(file);
			if(c == 'i')
			{// ���̓f�[�^
				fscanf_s(file,
						" %d %d %d %d %d %d %d %d %d %d",
						&m_dummyData[offset],
						&m_dummyData[offset + 1],
						&m_dummyData[offset + 2],
						&m_dummyData[offset + 3],
						&m_dummyData[offset + 4],
						&m_dummyData[offset + 5],
						&m_dummyData[offset + 6],
						&m_dummyData[offset + 7],
						&m_dummyData[offset + 8],
						&m_dummyData[offset + 9]);
				offset += 10;
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
void DummyDataLoad::Finalize(void)
{
}

// EOF
