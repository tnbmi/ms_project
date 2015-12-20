//*****************************************************************************
//
// 替え玉XInputゲームパッド入力 [dummyPadX.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_DUMMY_PAD_X_H
#define _MY_DUMMY_PAD_X_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\padX\padX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
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
