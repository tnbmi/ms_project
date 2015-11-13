//*****************************************************************************
//
// コマンドチェック処理マネージャ [commandmanager.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandmanager.h"
#include "..\common\safe.h"
#include "..\input\padX\padXManager.h"
#include "..\debugproc\debugproc.h"
#include "..\commandmanager\commandteam\commandteam.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Commandmanager::Commandmanager(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
}

//=============================================================================
// デストラクタ
//=============================================================================
Commandmanager::~Commandmanager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Commandmanager::Create(Commandmanager** outPointer, PadXManager* padXManager, Debugproc* debugproc)
{
	Commandmanager* pointer = new Commandmanager();
	if(!pointer->Initialize(padXManager, debugproc))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Commandmanager::Initialize(PadXManager* padXManager, Debugproc* debugproc)
{
	//----------------------------
	// コメント
	//----------------------------
	Commandteam::Create(&m_team[0]);
	m_team[0]->debugproc(debugproc);
	m_team[0]->playerset( padXManager->pad(0), padXManager->pad(1) );

	Commandteam::Create(&m_team[1]);
	m_team[1]->debugproc(debugproc);
	m_team[1]->playerset( padXManager->pad(2), padXManager->pad(3) );

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Commandmanager::Finalize(void)
{
	SafeFinalizeDelete(m_team[0]);
	SafeFinalizeDelete(m_team[1]);
}

//=============================================================================
// 更新
//=============================================================================
void Commandmanager::Update(void)
{
	m_team[0]->Update();
	m_team[1]->Update();
}

//=============================================================================
// 描画
//=============================================================================
void Commandmanager::Draw(void)
{
	m_team[0]->Draw();
	m_team[1]->Draw();
}

// EOF