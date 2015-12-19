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

	static bool Create(CommandDataLoad** outPointer, const char* filePath, int patternMax);
	bool Initialize(const char* filePath);
	void Finalize(void);

	unsigned int* commandDataA(void){return m_commandDataA;}
	unsigned int* commandDataB(void){return m_commandDataB;}

private:
	int m_patternMax;

	unsigned int* m_commandDataA;
	unsigned int* m_commandDataB;
};

//=============================================================================
#endif

// EOF
