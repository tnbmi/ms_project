//*****************************************************************************
//
// ソースのテンプレート [roadmanager.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "roadmanager.h"
#include "..\roadpolygon\roadpolygon.h"
#include "..\..\import\road\roadImport.h"
#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 定数定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const float _changeColorSpeed = 0.2f;
const int _checkMax = 4;

const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 150.0f, -3000.0f);

// 襖定数
	const D3DXVECTOR3 _husumaSize = D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT , 0.0f );
	const float _husumaSpeed = 0.5f;
	// 閉状態定数
	const D3DXVECTOR3 _husumaCloseLeftInitPos = D3DXVECTOR3( SCREEN_WIDTH * 0.5f - _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
		const D3DXVECTOR3 _husumaCloseRightInitPos = D3DXVECTOR3( SCREEN_WIDTH * 0.5f + _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
	// 開状態定数	   
		const D3DXVECTOR3 _husumaOpenLeftInitPos = D3DXVECTOR3( 0.0f + _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
		const D3DXVECTOR3 _husumaOpenRightInitPos = D3DXVECTOR3( SCREEN_WIDTH - _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
// アイコン定数
const D3DXVECTOR3 _iconSize = D3DXVECTOR3( 144.0f , 144.0f , 0.0f );
const D3DXVECTOR3 _iconInitPos = D3DXVECTOR3( SCREEN_WIDTH - _iconSize.x  , SCREEN_HEIGHT - _iconSize.y , 0.0f );
const float _iconRotSpeed = 0.5f;
// テキスト定数
const D3DXVECTOR3 _textSize = D3DXVECTOR3( 432.0f , 144.0f , 0.0f );
const D3DXVECTOR3 _textInitPos = D3DXVECTOR3( SCREEN_WIDTH - _iconSize.x - _textSize.x , SCREEN_HEIGHT - _textSize.y , 0.0f );
const float _textMoveLimit = _textInitPos.y + 10.0f;
const float _textSpeed = 0.1f;
//=============================================================================
// コンストラクタ
//=============================================================================
RoadManager::RoadManager(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_textSpeed = _textSpeed;
	m_state = ROAD_STATE_NONE;
	m_shader = nullptr;

}

//=============================================================================
// デストラクタ
//=============================================================================
RoadManager::~RoadManager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool RoadManager::Create(RoadManager** outPointer , LPDIRECT3DDEVICE9 device )
{
	RoadManager* pointer = new RoadManager();
	if(!pointer->Initialize( device ))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool RoadManager::Initialize(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// コメント
	//----------------------------
	if(!Shader::Create( &m_shader , device ) )
		return false;
	if(!RoadImport::Create( &m_import , device ))
		return false;

	if(!RoadPolygon::Create(&m_fadeLeft , device , m_import->texture(RoadImport::FADE_LEFT) , _husumaSize , RoadPolygon::POLYGON_CRITERIA_RIGHT))
		return false;
	if(!RoadPolygon::Create(&m_fadeRight , device , m_import->texture(RoadImport::FADE_RIGHT) , _husumaSize ,RoadPolygon::POLYGON_CRITERIA_LEFT))
		return false;
	if(!RoadPolygon::Create(&m_icon , device , m_import->texture(RoadImport::ICON) , _iconSize , RoadPolygon::POLYGON_CRITERIA_CENTER))
		return false;
	if(!RoadPolygon::Create(&m_text , device , m_import->texture(RoadImport::TEXT) , _textSize , RoadPolygon::POLYGON_CRITERIA_CENTER))
		return false;
	// 位置初期化
	InitializeClosePos();

	m_device = device;
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void RoadManager::Finalize(void)
{
	m_fadeLeft->Finalize();
	m_fadeRight->Finalize();
	m_icon->Finalize();
	m_text->Finalize();
	m_import->Finalize();
	delete m_import;
	m_shader->Finalize();
	delete m_shader;
	delete this;
}

//=============================================================================
// 更新
//=============================================================================
void RoadManager::Update(void)
{
	bool endFlag = false;
	switch( m_state )
	{
	case ROAD_STATE_CLOSE:
		Close();
		break;
	case ROAD_STATE_OPEN:
		Open();
		break;
	case ROAD_STATE_NONE:
		break;
	}
}

//=============================================================================
// 描画
//=============================================================================
void RoadManager::Draw(void)
{

	if( m_state != ROAD_STATE_NONE )
	{
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(64, 64, 128, 256), 1.0f, 0);

		LPD3DXCONSTANTTABLE vsc =m_shader->vsc(Shader::PAT_2D);
		LPD3DXCONSTANTTABLE psc =m_shader->psc(Shader::PAT_2D);
		// シェーダーをセット
		m_shader->SetShader(&vsc, &psc, (Shader::PATTERN)1);

		//----------------------------
		// Direct3Dによる描画
		//----------------------------
		if(SUCCEEDED(m_device->BeginScene()))	//開始
		{
			// 2D頂点設定
			m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

			m_fadeLeft->Draw(vsc,psc);
			m_fadeRight->Draw(vsc,psc);
			m_icon->Draw(vsc,psc);
			m_text->Draw(vsc,psc);
			m_device->EndScene();	// 終了
		}
		m_device->Present(nullptr, nullptr, nullptr, nullptr);

	}
}
//=============================================================================
// ポリゴン位置初期化
//=============================================================================
void RoadManager::InitializeOpenPos( void )
{
	m_fadeLeft->SetPosition( _husumaOpenLeftInitPos );
	m_fadeRight->SetPosition( _husumaOpenRightInitPos );
	m_icon->SetPosition( _iconInitPos );
	m_icon->SetRot(0.0f,0.0f,0.0f );
	m_text->SetPosition( _textInitPos );
}
//=============================================================================
// ポリゴン位置初期化
//=============================================================================
void RoadManager::InitializeClosePos( void )
{
	m_fadeLeft->SetPosition( _husumaCloseLeftInitPos );
	m_fadeRight->SetPosition( _husumaCloseRightInitPos );
	m_icon->SetPosition( _iconInitPos );
	m_icon->SetRot( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	m_text->SetPosition( _textInitPos );
}
//=============================================================================
// ローディング画面襖開ける
//=============================================================================
void RoadManager::Open( void )
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXCOLOR diff;
	int checkNum = 0;
	// 襖左更新
	pos = m_fadeLeft->GetPosition();
	if( pos.x > ( 0.0f - _husumaSize.x * 0.5f ) )
	{
		pos.x-=_husumaSpeed;
	}
	else
	{
		checkNum++;
	}
	m_fadeLeft->SetPosition( pos );
	m_fadeLeft->Update();

	// 襖右更新
	pos = m_fadeRight->GetPosition();
	if( pos.x < ( SCREEN_WIDTH + _husumaSize.x * 0.5f ) )
	{
		pos.x+=_husumaSpeed;
	}
	else
	{
		checkNum++;
	}
	m_fadeRight->SetPosition( pos );
	m_fadeRight->Update();

	// アイコン更新
	rot = m_icon->GetRot();
	rot.z+=_iconRotSpeed;
	m_icon->SetRot(rot);
	diff = m_icon->GetColor();
	if( diff.a > 0.0f )
	{
		diff.a -=_changeColorSpeed;
		m_icon->SetColor( diff );
	}
	else
	{
		checkNum++;
	}
	m_icon->Update();

	//テキスト更新
	pos = m_text->GetPosition();
	if( pos.y > -( _textInitPos.y + _textMoveLimit ) || pos.y < 0.0f )
	{
		m_textSpeed *= -1.0f;
	}
	pos.y += m_textSpeed;
	m_text->SetPosition( pos );
	diff = m_text->GetColor();
	if( diff.a > 0.0f )
	{
		diff.a -=_changeColorSpeed;
		m_text->SetColor(diff);
	}
	else
	{
		checkNum++;
	}
	m_text->Update();
	if( checkNum == _checkMax )
	{
		m_state = ROAD_STATE_NONE;
		InitializeOpenPos();
	}
}
//=============================================================================
// ローディング画面襖閉じる
//=============================================================================
void RoadManager::Close( void )
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXCOLOR diff;
	// 襖左更新
	pos = m_fadeLeft->GetPosition();
	if( pos.x < ( SCREEN_WIDTH * 0.5f ) - _husumaSize.x )
	{
		pos.x+=_husumaSpeed;
	}
	m_fadeLeft->SetPosition( pos );
	m_fadeLeft->Update();

	// 襖右更新
	pos = m_fadeRight->GetPosition();
	if( pos.x > ( SCREEN_WIDTH * 0.5f ) + _husumaSize.x )
	{
		pos.x-=_husumaSpeed;
	}
	m_fadeRight->SetPosition( pos );
	m_fadeRight->Update();

	// アイコン更新
	rot = m_icon->GetRot();
	rot.z+=_iconRotSpeed;
	m_icon->SetRot(rot);
	diff = m_icon->GetColor();
	if( diff.a < 1.0f )
	{
		diff.a += _changeColorSpeed;
	}
	m_icon->Update();
	pos = m_text->GetPosition();
	if( pos.y > -( _textInitPos.y + _textMoveLimit ) || pos.y < 0.0f )
	{
		m_textSpeed *= -1.0f;
	}
	pos.y += m_textSpeed;
	m_text->SetPosition( pos );
	diff = m_text->GetColor();
	if( diff.a < 1.0f )
	{
		diff.a += _changeColorSpeed;
	}
	m_text->Update();
}
// EOF
