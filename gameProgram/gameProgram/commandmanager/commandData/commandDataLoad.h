//*****************************************************************************
//
// コマンドデータのロード [commandDataLoad.h]
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
class CommandDataLoad
{
public:
	CommandDataLoad(int patternMax);
	~CommandDataLoad(void);

	static bool Create(CommandDataLoad** outPointer, int patternMax);
	bool Initialize(void);
	void Finalize(void);

	unsigned int* commandDataLoadA(void){return m_commandDataLoadA;}
	unsigned int* commandDataLoadB(void){return m_commandDataLoadB;}

private:
	int m_patternMax;
	int m_size;

	unsigned int* m_commandDataLoadA;
	unsigned int* m_commandDataLoadB;
};

//=============================================================================
#endif

// EOF
