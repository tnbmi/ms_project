//*****************************************************************************
//
// ダミー入力データのロード [dummyDataLoad.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_COMMAND_DATA_LOAD_H_
#define MY_COMMAND_DATA_LOAD_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DummyDataLoad
{
public:
	DummyDataLoad(int patternMax);
	~DummyDataLoad(void);

	static bool Create(DummyDataLoad** outPointer, const char* filePath, int patternMax);
	bool Initialize(const char* filePath);
	void Finalize(void);

	unsigned int* dummyData(void){return m_dummyData;}

private:
	int m_patternMax;

	unsigned int* m_dummyData;
};

//=============================================================================
#endif

// EOF
