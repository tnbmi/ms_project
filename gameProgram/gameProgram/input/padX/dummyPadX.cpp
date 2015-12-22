//*****************************************************************************
//
// 替え玉XInputゲームパッド入力 [dummyPadX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dummyPadX.h"
#include "..\..\common\random\random.h"

#include "..\..\debugproc\debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int _repeatMin		= 30;	// リピート開始カウント数
const int _repeatInterval	= 3;	// リピート間隔カウント数

const int _commandData[5] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	0x0000
};

//=============================================================================
// コンストラクタ
//=============================================================================
DummyPadX::DummyPadX(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	// ダミーフラグ
	m_dummy = true;

	// ダミーのデータ
	m_commandList = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
DummyPadX::~DummyPadX(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool DummyPadX::Create(PadX** outPointer, int no, int patternMax, unsigned int* commandList, int per)
{
	DummyPadX* pointer = new DummyPadX();
	if(!pointer->Initialize(no, patternMax, commandList, per))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool DummyPadX::Initialize(int no, int patternMax, unsigned int* commandList, int per)
{
	//----------------------------
	// 入力ナンバー
	//----------------------------
	m_no = no;

	//----------------------------
	// コマンド読み込み
	//----------------------------
	m_commandList = commandList;

	//----------------------------
	// 成功確率
	//----------------------------
	if(per < 0)
		per = 0;
	else if(per > 100)
		per = 100;

	m_per = (unsigned int)per;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void DummyPadX::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void DummyPadX::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc("***パッドNo:%1d******\n", m_no);
	m_debugproc->PrintDebugProc("！ダミー！　[確率：%d％]\n", m_per);
#endif

	//----------------------------
	// 入力情報更新
	//----------------------------
	XINPUT_GAMEPAD padInput;
	padInput.wButtons		= 0x0000;
	padInput.bLeftTrigger	= 0x0000;
	padInput.bRightTrigger	= 0x0000;
	padInput.sThumbLX		= 0x0000;
	padInput.sThumbLY		= 0x0000;
	padInput.sThumbRX		= 0x0000;
	padInput.sThumbRY		= 0x0000;

	if(m_pressFlg)
	{
		WORD com = _commandData[m_commandList[m_commandPrev*10 + m_commandCnt]];

		if(Random::Rand()%100 <= m_per)
			padInput.wButtons = com;
		else
		{
			WORD missCom = _commandData[Random::Rand()%4];
			padInput.wButtons = missCom & ~com;
		}

		m_pressFlg = false;
	}

	// トリガー
	m_trigger.wButtons		= (padInput.wButtons	  ^ m_state.wButtons)		& padInput.wButtons;
	m_trigger.bLeftTrigger	= (padInput.bLeftTrigger  ^ m_state.bLeftTrigger)	& padInput.bLeftTrigger;
	m_trigger.bRightTrigger	= (padInput.bRightTrigger ^ m_state.bRightTrigger)	& padInput.bRightTrigger;
	m_trigger.sThumbLX		= (padInput.sThumbLX	  ^ m_state.sThumbLX)		& padInput.sThumbLX;
	m_trigger.sThumbLY		= (padInput.sThumbLY	  ^ m_state.sThumbLY)		& padInput.sThumbLY;
	m_trigger.sThumbRX		= (padInput.sThumbRX	  ^ m_state.sThumbRX)		& padInput.sThumbRX;
	m_trigger.sThumbRY		= (padInput.sThumbRY	  ^ m_state.sThumbRY)		& padInput.sThumbRY;

	// リリース
	m_release.wButtons		= (padInput.wButtons	  ^ m_state.wButtons)		& m_state.wButtons;
	m_release.bLeftTrigger	= (padInput.bLeftTrigger  ^ m_state.bLeftTrigger)	& m_state.bLeftTrigger;
	m_release.bRightTrigger	= (padInput.bRightTrigger ^ m_state.bRightTrigger)	& m_state.bRightTrigger;
	m_release.sThumbLX		= (padInput.sThumbLX	  ^ m_state.sThumbLX)		& m_state.sThumbLX;
	m_release.sThumbLY		= (padInput.sThumbLY	  ^ m_state.sThumbLY)		& m_state.sThumbLY;
	m_release.sThumbRX		= (padInput.sThumbRX	  ^ m_state.sThumbRX)		& m_state.sThumbRX;
	m_release.sThumbRY		= (padInput.sThumbRY	  ^ m_state.sThumbRY)		& m_state.sThumbRY;

	// プレス
	m_state = padInput;

#ifdef _DEBUG
	// パッド表示情報更新
	m_debugproc->PrintDebugProc("---入力コマンド---\n");
	m_debugproc->PrintDebugProc("上:%d 下:%d 左:%d 右:%d\n",
								buttonPress(XINPUT_GAMEPAD_DPAD_UP),
								buttonPress(XINPUT_GAMEPAD_DPAD_DOWN),
								buttonPress(XINPUT_GAMEPAD_DPAD_LEFT),
								buttonPress(XINPUT_GAMEPAD_DPAD_RIGHT));
#endif
}

// [EOF]
