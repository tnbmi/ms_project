//*****************************************************************************
//
// チームのコマンド管理 [commandteam.cpp]
// Author : KEN MATSUURA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "commandteam.h"
#include "..\..\common\safe.h"

#include "..\..\debugproc\debugproc.h"

#include "..\..\input\padX\padX.h"

#include "..\..\list\objectList\objectList.h"
#include "..\..\list\updateList\updateList.h"
#include "..\..\list\drawList\drawListManager.h"

#include "..\..\objectBase\polygon2D\polygon2D.h"
#include "..\..\import\game\gameImport.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int	_time_penalty = 30;
const int	_command_max = 6;
const int	_command_min = 4;
const int	_upper_limit = 2;
const char*	_comtex[6] = {"UP\n","Y\n","RIGHT\n","DOWN\n","A\n","X\n"};

//=============================================================================
// コンストラクタ
//=============================================================================
Commandteam::Commandteam(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_command = nullptr;

	m_pad[0]			= nullptr;
	m_pad[1]			= nullptr;

	m_command_count		= 0;
	m_time_penalty		= 0;
	m_command_long		= 0;
	m_flag_lose			= false;

	m_objectList		= nullptr;
	m_updateList		= nullptr;
	m_drawListManager	= nullptr;

	m_debugproc			= nullptr;
	m_device			= nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Commandteam::~Commandteam(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Commandteam::Create(Commandteam** outPointer,
						 ObjectList* objList,
						 UpdateList* updList,
						 DrawListManager* drwList,
						 LPDIRECT3DDEVICE9 device,
						 GameImport* import)
{
	Commandteam* pointer = new Commandteam();
	if(!pointer->Initialize(objList, updList, drwList, device, import))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Commandteam::Initialize(ObjectList* objList,
							 UpdateList* updList,
							 DrawListManager* drwList,
							 LPDIRECT3DDEVICE9 device,
							 GameImport* import)
{
	//----------------------------
	// ステータス
	//----------------------------
	m_command_long = _command_max;
	m_device = device;
	m_objectList = objList;
	m_updateList = updList;
	m_drawListManager = drwList;
	m_import = import;

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
void Commandteam::Finalize(void)
{

}

//=============================================================================
// 更新
//=============================================================================
bool Commandteam::Update(void)
{
#ifdef _DEBUG
	m_debugproc->PrintDebugProc( _comtex[m_command_count] );
#endif

	if(m_time_penalty > 0)		// ペナルティタイム
	{
		m_time_penalty--;
#ifdef _DEBUG
		m_debugproc->PrintDebugProc( "ERROR!\n" );
#endif
	}
	else						// 通常処理
	{
		int current_user = m_command_count % 2;

		if( m_pad[current_user]->buttonTrigger( 0xf00f ) )
		{
			if( m_pad[current_user]->buttonTrigger( *(m_command + m_command_count) ) )
				m_command_count++;
			else
				m_time_penalty = _time_penalty;
		}
	}

	if(m_command_count >= m_command_long)
	{
		m_command_count = 0;

		if(m_flag_lose)
			m_command_long = _command_min;

		return true;
	}

	return false;
}

//=============================================================================
// 描画
//=============================================================================
void Commandteam::Draw(void)
{
}

//=============================================================================
// フラグセット
//=============================================================================
void Commandteam::SetFragLose(bool flag)
{
	m_flag_lose = flag;

	if(flag)
	{
		if(m_command_count <= _upper_limit)
		{
			m_command_long = _command_min;
		}
	}
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
bool Commandteam::InitObject(void)
{
	//----------------------------
	// 2Dポリゴン
	//----------------------------
	Polygon2D* poly2d;
	if(!Polygon2D::Create(&poly2d, m_device, m_objectList, m_import->texture(GameImport::BUTTON_A)))
		return false;
	m_updateList->Link(poly2d);
	m_drawListManager->Link(poly2d, 4, Shader::PAT_2D);
	poly2d->pos(200.0f, 200.0f, 0.0f);
	poly2d->scl(72.0f, 72.0f, 0.0f);

	return true;
}

// EOF
