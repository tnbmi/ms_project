//*****************************************************************************
//
// ゲームプレイヤー [player.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef PLAYER_H_
#define PLAYER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

class FbxModel;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class PlayerState;

class Player
{
public:

	Player( FbxModel *parent,FbxModel *child );
	virtual ~Player(void);

	static bool Create( Player** outPointer, FbxModel *parent,FbxModel *child );
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


	

private:
	FbxModel *m_parent;
	FbxModel *m_child;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_scl;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_offsetPos;
	D3DXVECTOR3 m_offsetRot;

	//ステート情報
	PlayerState *m_state;
	
};

//プレイヤー状態
class PlayerState
{
public:

	PlayerState(){}
	virtual ~PlayerState(){}
	virtual void Update( Player *player ) = 0;

private:
};


//待機
class PlayerWaiting : public PlayerState
{

};

//攻撃
class PlayerAttack : public PlayerState
{

};

//防御
class PlayerDefence : public PlayerState
{

};

//勝利
class PlayerWin : public PlayerState
{

};

//負け
class PlaterLose : public PlayerState
{

};

//=============================================================================
#endif

// EOF
