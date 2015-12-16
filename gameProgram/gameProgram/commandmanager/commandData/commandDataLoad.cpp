//*****************************************************************************
//
// コマンドデータのロード [commandDataLoad.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandDataLoad.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CommandDataLoad::CommandDataLoad(int patternMax)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_patternMax = patternMax;
	m_size		 = patternMax * 10;

	m_commandDataA = new unsigned int[m_size];
	m_commandDataB = new unsigned int[m_size];

	for(int cnt = 0; cnt < m_size; ++cnt)
	{
		m_commandDataA[cnt] = 0;
		m_commandDataB[cnt] = 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CommandDataLoad::~CommandDataLoad(void)
{
	SafeDeleteArray(m_commandDataA);
	SafeDeleteArray(m_commandDataB);
}

//=============================================================================
// 生成
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
// 初期化
//=============================================================================
bool CommandDataLoad::Initialize(const char* filePath)
{
	FILE*	file;
	char	c;
	int		offsetA = 0;
	int		offsetB = 0;

	//----------------------------
	// ファイルオープン
	//----------------------------
	fopen_s(&file, filePath, "r");

	if(file == nullptr)
		return false;

	//----------------------------
	// コマンドデータ読み込み
	//----------------------------
	while((c = fgetc(file)) != EOF)
	{
		if(c == '/')
		{
			c = fgetc(file);
			if(c == 'a')
			{// プレイヤーA
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
			{// プレイヤーB
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
	// ファイルクローズ
	//----------------------------
	fclose(file);

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void CommandDataLoad::Finalize(void)
{
}

// EOF
