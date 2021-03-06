//*****************************************************************************
//
// プレイヤー [player.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\common\complement.h"
#include "..\..\list\drawList\drawListManager.h"
#include "..\..\list\updateList\updateList.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Player::Player( FbxModel *parent,FbxModel *child,FbxModel *secondChild )
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_parent = parent;
	m_child  = child;
	m_secondChild = secondChild;
}

//=============================================================================
// デストラクタ
//=============================================================================
Player::~Player(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Player::Create(Player** outPointer, FbxModel *parent,FbxModel *child,FbxModel *secondChild)
{
	Player* pointer = new Player( parent,child,secondChild );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

bool Player::Create( Player** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList, int priority  , ObjectBase::OBJECT_TYPE type,
						const char *parentModelPath,const char *childModelPath,const char *secondChildModelPath,FbxTexImport *import )
{
	FbxModel *parent,*child,*secondChild;
	if( !FbxModel::Create( &parent,device,objectList,priority,type,parentModelPath,import ) )
		return false;

	updateList->Link( parent );
	drawList->Link( parent,0,Shader::PAT_FBX );

	if( !FbxModel::Create( &child,device,objectList,priority,type,childModelPath,import ) )
		return false;

	updateList->Link( child );
	drawList->Link( child,0,Shader::PAT_FBX );

	if( !FbxModel::Create( &secondChild,device,objectList,priority,type,secondChildModelPath,import ) )
		return false;

	updateList->Link( secondChild );
	drawList->Link( secondChild,0,Shader::PAT_FBX );

	Player* pointer = new Player( parent,child,secondChild );
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Player::Initialize(void)
{
	//----------------------------
	// メンバ初期化
	//----------------------------
	m_pos = D3DXVECTOR3(0,0,0);
	m_rot = D3DXVECTOR3(0,0,0);
	m_scl = D3DXVECTOR3(-1,1,1);
	m_offsetPos = D3DXVECTOR3(0,0,0);
	m_offsetRot = D3DXVECTOR3(0,0,0);
	m_secondOffsetPos = D3DXVECTOR3(0,0,0);
	m_secondOffsetRot = D3DXVECTOR3(0,0,0);
	m_compTime = 0;
	m_elepsed = 0;
	m_stPos = D3DXVECTOR3(0,0,0);
	m_edPos = D3DXVECTOR3(0,0,0);
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Player::Finalize(void)
{

}

//=============================================================================
// 更新
//=============================================================================
void Player::Update(void)
{
	//更新
	m_pos =LerpVec3( m_stPos,m_edPos,0,m_compTime,m_elepsed,Cube );

	m_elepsed++;
	
	if( m_elepsed >= m_compTime )
	{
		m_elepsed = m_compTime;
	}

	//親の位置更新
	m_parent->pos( m_pos );
	m_parent->rot( m_rot );
	m_parent->scl( m_scl );

	//子の位置更新
	m_child->pos( m_pos + m_offsetPos );
	m_child->rot( m_rot + m_offsetRot );
	m_child->scl( m_scl );

	//第二子の更新
	m_secondChild->pos( m_pos + m_secondOffsetPos );
	m_secondChild->rot( m_rot + m_secondOffsetRot );
	m_secondChild->scl( m_scl );
}

//============================================================================
// Move
//============================================================================

void Player::Move( const D3DXVECTOR3 &stPos,const D3DXVECTOR3 &edPos,const float compTime )
{
	m_stPos = stPos;
	m_pos = stPos;
	m_edPos = edPos;
	m_compTime = compTime;
	m_elepsed = 0;
}

//============================================================================
// StartAnimation
//============================================================================

void Player::StartAnimationParent( const int startKeyFrame,const int endKeyFrame,const bool isRoop )
{
	m_parent->StartAnimation( startKeyFrame,endKeyFrame,isRoop );
}

void Player::StartAnimationChild( const int startKeyFrame,const int endKeyFrame,const bool isRoop )
{
	m_child->StartAnimation( startKeyFrame,endKeyFrame,isRoop );
}

void Player::StartAnimationSecondChild( const int startKeyFrame,const int endKeyFrame,const bool isRoop )
{
	m_secondChild->StartAnimation( startKeyFrame,endKeyFrame,isRoop );
}

void Player::ApplySclRotPos()
{
	//親の位置更新
	m_parent->pos( m_pos );
	m_parent->rot( m_rot );
	m_parent->scl( m_scl );

	//子の位置更新
	m_child->pos( m_pos + m_offsetPos );
	m_child->rot( m_rot + m_offsetRot );
	m_child->scl( m_scl );

	//第二子の更新
	m_secondChild->pos( m_pos + m_secondOffsetPos );
	m_secondChild->rot( m_rot + m_secondOffsetRot );
	m_secondChild->scl( m_scl );

	m_parent->Update();
	m_child->Update();
	m_secondChild->Update();
}

// EOF