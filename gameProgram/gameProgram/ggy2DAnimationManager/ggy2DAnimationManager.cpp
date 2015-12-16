//*****************************************************************************
//
// �����[ [ggy2DAnimationManager.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "ggy2DAnimationManager.h"
#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\tex2DAnimation\tex2DAnimation.h"
#include "..\objectBase\polygon2D\polygon2D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Ggy2DAnimationManager::Ggy2DAnimationManager( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_device = device;
	m_objectList = objectList;
	m_updateList = updateList;
	m_drawListManager = drawList;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Ggy2DAnimationManager::~Ggy2DAnimationManager(void)
{
}

//=============================================================================
// ����
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
// ������
//=============================================================================
bool Ggy2DAnimationManager::Initialize(void)
{

	Polygon2D *p;
	Polygon2D::Create( &p,m_device,m_objectList,NULL,ObjectBase::TYPE_2D );
	
	m_updateList->Link( p );
	m_drawListManager->Link( p,3,Shader::PAT_2D );

	m_poly = p;

	//----------------------------
	// �R�����g
	//----------------------------
	m_animArray = new Tex2DAnimation[ _animMax ];

	for( int i = 0 ; i < _animMax ; i++ )
	{
		m_animArray[i].Set2DPolygon( p );
	}

	m_animArray[0].SetAnimationData( 10,10,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.1f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.1f,1.0f),D3DXVECTOR2(0.1f,0.0f));
	m_animArray[8].SetAnimationData( 3,20,D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.05f,0.0f),D3DXVECTOR2(0.0f,1.0f),D3DXVECTOR2(0.05f,1.0f),D3DXVECTOR2(0.05f,0.0f));

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Ggy2DAnimationManager::Finalize(void)
{
	delete []m_animArray;
}

//=============================================================================
// �X�V
//=============================================================================
void Ggy2DAnimationManager::Update(void)
{
	m_animArray[m_animIdx].Update();

	m_poly->scl(m_scl);
	m_poly->pos(m_pos);
}

//=============================================================================
// �`��
//=============================================================================
void Ggy2DAnimationManager::Draw(void)
{
}

//=============================================================================
//
//=============================================================================
void Ggy2DAnimationManager::SetTexture( const int idx,LPDIRECT3DTEXTURE9 tex )
{
	m_animArray[idx].SetTexture( tex );
}

//=============================================================================
//
//=============================================================================

void Ggy2DAnimationManager::StartAnimation( const int idx,bool isRoop )
{
	m_animArray[idx].StartAnimaton(isRoop);
	m_animIdx = idx;
}


// EOF
