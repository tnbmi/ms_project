//*****************************************************************************
//
// �Q�[���v���C���[ [player.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef PLAYER_H_
#define PLAYER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\..\objectBase\objectBase.h"



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FbxModel;
class PlayerState;
class DrawListManager;
class UpdateList;

class Player
{
public:

	Player( FbxModel *parent,FbxModel *child,FbxModel *secondChild );
	virtual ~Player(void);

	static bool Create( Player** outPointer, FbxModel *parent,FbxModel *child,FbxModel *secondChild );
	static bool Create( Player** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList, int priority  , ObjectBase::OBJECT_TYPE type,
						const char *parentModelPath,const char *childModelPath,const char *secondChildModelPath );
	bool Initialize(void);
	void Finalize(void);
	void Update(void);

	D3DXVECTOR3 pos(){ return m_pos; }
	void        pos( const D3DXVECTOR3 &pos ){ m_pos = pos; }

	D3DXVECTOR3 scl(){ return m_scl; }
	void        scl( const D3DXVECTOR3 &scl ){ m_scl = scl; }

	D3DXVECTOR3 rot(){ return m_rot; }
	void        rot( const D3DXVECTOR3 &rot ){ m_rot = rot; }

	D3DXVECTOR3 offsetPos(){ return m_offsetPos; }
	void        offsetPos( const D3DXVECTOR3 &offsetPos ){ m_offsetPos = offsetPos; }

	D3DXVECTOR3 offsetRot(){ return m_offsetRot; }
	void        offsetRot( const D3DXVECTOR3 &offsetRot ){ m_offsetRot = offsetRot; }


	D3DXVECTOR3 secondOffsetPos(){ return m_secondOffsetPos; }
	void        secondOffsetPos( const D3DXVECTOR3 &offsetPos ){ m_secondOffsetPos = offsetPos; }

	D3DXVECTOR3 secondOffsetRot(){ return m_secondOffsetRot; }
	void        secondOffsetRot( const D3DXVECTOR3 &offsetRot ){ m_secondOffsetRot = offsetRot; }

	//�ړ��֐��@�Z�b�^�[�Ƃ��Ă����g���������� 
	//�J�n�ʒu�@�I���ʒu�@���t���[���ŕ⊮����񂾂x�n
	void Move( const D3DXVECTOR3 &stPos,const D3DXVECTOR3 &edPos,const float compTime );

	

private:
	FbxModel *m_parent;
	FbxModel *m_child;
	FbxModel *m_secondChild;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_scl;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_offsetPos;
	D3DXVECTOR3 m_offsetRot;
	D3DXVECTOR3 m_secondOffsetPos;
	D3DXVECTOR3 m_secondOffsetRot;

	D3DXVECTOR3 m_stPos;
	D3DXVECTOR3 m_edPos;
	float m_elepsed;
	float m_compTime;

	//�X�e�[�g���
	PlayerState *m_state;
	
};

//�v���C���[���
class PlayerState
{
public:

	PlayerState(){}
	virtual ~PlayerState(){}
	virtual void Update( Player *player ) = 0;

private:
};


//�ҋ@
class PlayerWaiting : public PlayerState
{

};

//�U��
class PlayerAttack : public PlayerState
{

};

//�h��
class PlayerDefence : public PlayerState
{

};

//����
class PlayerWin : public PlayerState
{

};

//����
class PlaterLose : public PlayerState
{

};

//=============================================================================
#endif

// EOF
