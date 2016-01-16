//*****************************************************************************
//
// じじー [ggy2DAnimationManager.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "ggy2DAnimationManager.h"
#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\tex2DAnimation\tex2DAnimation.h"
#include "..\objectBase\polygon2D\polygon2D.h"
#include "..\common\safe.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Ggy2DAnimationManager::Ggy2DAnimationManager( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;

	m_animIdx = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
Ggy2DAnimationManager::~Ggy2DAnimationManager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Ggy2DAnimationManager::Create(Ggy2DAnimationManager** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList
						)
{
	Ggy2DAnimationManager* pointer = new Ggy2DAnimationManager( device,objectList,updateList,drawList);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Ggy2DAnimationManager::Initialize(void)
{	
	Polygon2D *p;
	Polygon2D::Create( &p,m_device,m_objectList,NULL,ObjectBase::TYPE_2D );
	
	m_updateList->Link( p );
	m_drawListManager->Link( p,3,Shader::PAT_2D );

	m_poly = p;

	//----------------------------
	// コメント
	//----------------------------
	//m_animArray = new Tex2DAnimation*[ _animMax ];

	for( int i = 0 ; i < _animMax ; i++ )
	{
		Tex2DAnimation::Create( &m_animArray[i] );
		m_animArray[i]->Set2DPolygon( p );
	}



	m_animArray[0]->SetAnimationData( 3.0f,10,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.1f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.1f,1.0f),D3DXVECTOR2(0.1f,0.0f));
	m_animArray[1]->SetAnimationData( 1.5f,20,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.05f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.05f,1.0f),D3DXVECTOR2(0.05f,0.0f));
	m_animArray[2]->SetAnimationData( 1.5f,20,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.05f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.05f,1.0f),D3DXVECTOR2(0.05f,0.0f));
	m_animArray[3]->SetAnimationData( 3.0f,10,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.1f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.1f,1.0f),D3DXVECTOR2(0.1f,0.0f));
	m_animArray[4]->SetAnimationData( 3.0f,10,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.1f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.1f,1.0f),D3DXVECTOR2(0.1f,0.0f));
	m_animArray[5]->SetAnimationData( 3.0f,10,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.1f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.1f,1.0f),D3DXVECTOR2(0.1f,0.0f));
	m_animArray[6]->SetAnimationData( 3.0f,10,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.1f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.1f,1.0f),D3DXVECTOR2(0.1f,0.0f));
	m_animArray[7]->SetAnimationData( 3.0f,20,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.05f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.05f,1.0f),D3DXVECTOR2(0.05f,0.0f));
	m_animArray[8]->SetAnimationData( 3.0f,20,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.05f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.05f,1.0f),D3DXVECTOR2(0.05f,0.0f));

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Ggy2DAnimationManager::Finalize(void)
{
	for( int i = 0 ; i <_animMax;i++ )
	{
		SafeDelete( m_animArray[i] );
	}

	//SafeDeleteArray( m_animArray );
}

//=============================================================================
// 更新
//=============================================================================
void Ggy2DAnimationManager::Update(void)
{
	m_animArray[m_animIdx]->Update();

	m_poly->scl(m_scl);
	m_poly->pos(m_pos);
}

//=============================================================================
// 描画
//=============================================================================
void Ggy2DAnimationManager::Draw(void)
{
}

//=============================================================================
//
//=============================================================================
void Ggy2DAnimationManager::SetTexture( const int idx,LPDIRECT3DTEXTURE9 tex )
{
	m_animArray[idx]->SetTexture( tex );
}

//=============================================================================
//
//=============================================================================

void Ggy2DAnimationManager::StartAnimation( const int idx,bool isRoop )
{
	m_animArray[idx]->StartAnimation(isRoop);
	m_animIdx = idx;//FPSに影響なし

	LPDIRECT3DTEXTURE9 tex = m_animArray[idx]->texture();

	m_poly->texture(tex);//これ？
}

bool Ggy2DAnimationManager::isEndAnimation()
{
	return m_animArray[m_animIdx]->isEndAnimation();
}


// EOF
