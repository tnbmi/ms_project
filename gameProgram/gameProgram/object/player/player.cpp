//*****************************************************************************
//
// �v���C���[ [player.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "player.h"
#include "..\..\objectBase\fbxModel\fbxModel.h"
#include "..\..\common\complement\complement.h"
#include "..\..\list\drawList\drawListManager.h"
#include "..\..\list\updateList\updateList.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Player::Player( FbxModel *parent,FbxModel *child,FbxModel *secondChild )
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_parent = parent;
	m_child  = child;
	m_secondChild = secondChild;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Player::~Player(void)
{
}

//=============================================================================
// ����
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
						const char *parentModelPath,const char *childModelPath,const char *secondChildModelPath )
{
	FbxModel *parent,*child,*secondChild;
	if( !FbxModel::Create( &parent,device,objectList,priority,type,parentModelPath ) )
		return false;

	updateList->Link( parent );
	drawList->Link( parent,0,Shader::PAT_FBX );

	if( !FbxModel::Create( &child,device,objectList,priority,type,childModelPath ) )
		return false;

	updateList->Link( child );
	drawList->Link( child,0,Shader::PAT_FBX );

	if( !FbxModel::Create( &secondChild,device,objectList,priority,type,secondChildModelPath ) )
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
// ������
//=============================================================================
bool Player::Initialize(void)
{
	//----------------------------
	// �����o������
	//----------------------------
	m_pos = D3DXVECTOR3(0,0,0);
	m_rot = D3DXVECTOR3(0,0,0);
	m_scl = D3DXVECTOR3(1,1,1);
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
// �I��
//=============================================================================
void Player::Finalize(void)
{

}

//=============================================================================
// �X�V
//=============================================================================
void Player::Update(void)
{
	//�X�V
	m_pos =LerpVec3( m_stPos,m_edPos,0,m_compTime,m_elepsed,Cube );

	m_elepsed++;
	
	if( m_elepsed >= m_compTime )
	{
		m_elepsed = m_compTime;
	}

	//�e�̈ʒu�X�V
	m_parent->pos( m_pos );
	m_parent->rot( m_rot );
	m_parent->scl( m_scl );

	//�q�̈ʒu�X�V
	m_child->pos( m_pos + m_offsetPos );
	m_child->rot( m_rot + m_offsetRot );
	m_child->scl( m_scl );

	//���q�̍X�V
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
	m_edPos = edPos;
	m_compTime = compTime;
	m_elepsed = 0;
}

// EOF