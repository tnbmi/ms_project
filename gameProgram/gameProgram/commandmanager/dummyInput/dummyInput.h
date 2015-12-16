//*****************************************************************************
//
// プレイヤー替え玉 [dummyInput.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_DUMMY_INPUT_H_
#define MY_DUMMY_INPUT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DummyDataLoad;

class DummyInput
{
public:
	DummyInput(void);
	~DummyInput(void);

	static bool Create(DummyInput** outPointer, int patternMax);
	bool Initialize(int patternMax);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	DummyDataLoad*	m_dummyDataLoad;
	unsigned int*	m_dummy_list;
};

//=============================================================================
#endif

// EOF
