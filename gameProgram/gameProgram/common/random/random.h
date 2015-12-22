//*****************************************************************************
//
// 乱数クラス [random.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_RANDOM_H_
#define MY_RANDOM_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Random
{
public:
	Random(void){};
	~Random(void){};

	static void SetRandom(unsigned int value);
	static unsigned int Rand(void);
	static unsigned int Rand(unsigned int value);

	static int Rand(int minimum, int maximum);
	static float Rand(float minimum, float maximum);

private:
	static unsigned int m_value1;
	static unsigned int m_value2;
	static unsigned int m_value3;
	static unsigned int m_value4;
};

//=============================================================================
#endif

// EOF
