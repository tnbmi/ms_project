//*****************************************************************************
//
// ゲーム時間管理 [timeManager.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "timeManager.h"
#include "..\common\safe.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\import\game\gameImport.h"

#include "..\number\number.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const D3DXVECTOR2 _time_poly_size = D3DXVECTOR2(45.0f, 80.0f);
const D3DXVECTOR3 _time_pos = D3DXVECTOR3(SCREEN_WIDTH*0.5f - _time_poly_size.x*0.5f, 75.0f, 0.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
TimeManager::TimeManager(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_import = nullptr;
	m_device = nullptr;

	m_objectList = nullptr;
	m_updateList = nullptr;
	m_drawListManager = nullptr;

	m_time = 0;
	m_poly_list[0] = nullptr;
	m_poly_list[1] = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
TimeManager::~TimeManager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool TimeManager::Create(TimeManager** outPointer,
						 ObjectList* objList,
						 UpdateList* updList,
						 DrawListManager* drwList,
						 LPDIRECT3DDEVICE9 device,
						 Import* import,
						 int time)
{
	TimeManager* pointer = new TimeManager();
	if(!pointer->Initialize(objList, updList, drwList, device, import, time))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool TimeManager::Initialize(ObjectList* objList,
							 UpdateList* updList,
							 DrawListManager* drwList,
							 LPDIRECT3DDEVICE9 device,
							 Import* import,
							 int time)
{
	//----------------------------
	// ステータス
	//----------------------------
	m_device = device;
	m_objectList = objList;
	m_updateList = updList;
	m_drawListManager = drwList;
	m_import = import;
	m_time = time;

	//----------------------------
	// オブジェクト
	//----------------------------
	if(!InitObject())
		return false;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void TimeManager::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
bool TimeManager::Update(void)
{
	m_time--;

	int j = 0;
	for(int i = 2; i > 0; i--)
	{
		m_poly_list[j]->SetNumber((int)( ((m_time/60) % ( (int)pow(10.0f, i) ) ) / (int)pow(10.0f, i - 1) ));
		j++;
	}

	if(m_time <= 0)
		return true;
	return false;
}

//=============================================================================
// 描画
//=============================================================================
void TimeManager::Draw(void)
{
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool TimeManager::InitObject(void)
{
	if(!Number::Create(&m_poly_list[0], m_device, m_objectList, m_import->texture(GameImport::NUMBER_YELLOW)))
		return false;
	m_updateList->Link(m_poly_list[0]);
	m_drawListManager->Link(m_poly_list[0], 4, Shader::PAT_2D);
	m_poly_list[0]->pos(SCREEN_WIDTH / 2 - _time_poly_size.x/3*2, _time_pos.y, 0.0f);
	m_poly_list[0]->scl(_time_poly_size.x, _time_poly_size.y, 0.0f);

	if(!Number::Create(&m_poly_list[1], m_device, m_objectList, m_import->texture(GameImport::NUMBER_YELLOW)))
		return false;
	m_updateList->Link(m_poly_list[1]);
	m_drawListManager->Link(m_poly_list[1], 4, Shader::PAT_2D);
	m_poly_list[1]->pos(SCREEN_WIDTH / 2 + _time_poly_size.x/3*2, _time_pos.y, 0.0f);
	m_poly_list[1]->scl(_time_poly_size.x, _time_poly_size.y, 0.0f);

	return true;
}

// EOF
