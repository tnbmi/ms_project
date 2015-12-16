//*****************************************************************************
//
// ダミー入力データのロード [dummyData.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dummyDataLoad.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
DummyDataLoad::DummyDataLoad(int patternMax)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_patternMax = patternMax;
	m_size		 = patternMax * 10;

	m_dummyData = new unsigned int[m_size];

	for(int cnt = 0; cnt < m_size; ++cnt)
	{
		m_dummyData[cnt] = 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
DummyDataLoad::~DummyDataLoad(void)
{
	SafeDeleteArray(m_dummyData);
}

//=============================================================================
// 生成
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
// 初期化
//=============================================================================
bool DummyDataLoad::Initialize(const char* filePath)
{
	FILE*	file;
	char	c;
	int		offset = 0;

	//----------------------------
	// ファイルオープン
	//----------------------------
	fopen_s(&file, filePath, "r");

	if(file == nullptr)
		return false;

	//----------------------------
	// ダミー入力データ読み込み
	//----------------------------
	while((c = fgetc(file)) != EOF)
	{
		if(c == '/')
		{
			c = fgetc(file);
			if(c == 'i')
			{// 入力データ
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
	// ファイルクローズ
	//----------------------------
	fclose(file);

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void DummyDataLoad::Finalize(void)
{
}

// EOF
