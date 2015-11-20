//*****************************************************************************
//
// ゲームフェーズ [game.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_GAME_H_
#define MY_GAME_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class GameImport;
class Camera;
class ObjectList;
class UpdateList;
class DrawListManager;
class Commandmanager;

class Game : public Phase
{
public:
	Game(LPDIRECT3DDEVICE9 device);
	virtual ~Game(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	GameImport*	m_import;
	Camera*		m_camera;
	Commandmanager*	m_command_manager;
};

//=============================================================================
#endif

// EOF
