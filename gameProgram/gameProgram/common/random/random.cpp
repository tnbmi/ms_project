//*****************************************************************************
//
// 乱数クラス [random.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "random.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned int Random::m_value1 = 207555608;
unsigned int Random::m_value2 = 4002963216;
unsigned int Random::m_value3 = 4126398668;
unsigned int Random::m_value4 = 2552396014;

//=============================================================================
// 乱数設定
//=============================================================================
void Random::SetRandom(unsigned int value)
{
	//----------------------------
	// 乱数用の値を設定
	//----------------------------
	m_value1 = value;
	m_value2 = Rand(m_value1);
	m_value3 = Rand(m_value2);
	m_value4 = Rand(m_value3);
}

//=============================================================================
// 乱数排出関数
// 事前に設定された値4つから乱数排出
//=============================================================================
unsigned int Random::Rand(void)
{
	unsigned int temp = m_value1 ^ (m_value1 << 11);

	m_value1 = m_value2;
	m_value2 = m_value3;
	m_value3 = m_value4;
	m_value4 = (m_value4 ^ (m_value4 >> 19)) ^ (temp ^ (temp >> 8));

	return m_value4;
}

//=============================================================================
// 乱数排出関数
// 任意の数字を元に乱数排出
//=============================================================================
unsigned int Random::Rand(unsigned int value)
{
	unsigned int answer = value;

	answer = answer ^ (answer << 13);
	answer = answer ^ (answer >> 17);
	answer = answer ^ (answer << 5);

	return answer;
}

//=============================================================================
// 乱数排出関数
// 事前に設定された値4つから下限〜上限で乱数排出
//=============================================================================
int Random::Rand(int minimum, int maximum)
{
	//----------------------------
	// 上限下限のチェック
	//----------------------------
	int chkMax = maximum;

	maximum = max(minimum, maximum);
	minimum = min(minimum, chkMax);

	//----------------------------
	// 事前に設定された値4つから乱数排出
	//----------------------------
	unsigned int rendomValue = Rand();

	//----------------------------
	// 範囲内に丸め込み
	//----------------------------
	unsigned int diff = maximum - minimum;
	int answer = rendomValue % diff + minimum;

	return answer;
}

//=============================================================================
// 乱数排出関数
// 事前に設定された値4つから下限〜上限で乱数排出
//=============================================================================
float Random::Rand(float minimum, float maximum)
{
	//----------------------------
	// 上限下限のチェック
	//----------------------------
	maximum *= 10000;
	minimum *= 10000;

	float chkMax = maximum;
	maximum = max(minimum, maximum);
	minimum = min(minimum, chkMax);

	//----------------------------
	// 事前に設定された値4つから乱数排出
	//----------------------------
	unsigned int rendomValue = Rand();

	//----------------------------
	// 範囲内に丸め込み
	//----------------------------
	unsigned int diff = (int)maximum - (int)minimum;
	float answer = rendomValue % diff + minimum;
	answer *= 0.0001f;

	return answer;
}

// EOF
