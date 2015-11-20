//*****************************************************************************
//
// �Q�[���t�F�[�Y [game.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_GAME_H_
#define MY_GAME_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class GameImport;
class Camera;
class ObjectList;
class UpdateList;
class DrawListManager;
class Commandmanager;
class EffectManager;
class Player;

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

	EffectManager *m_effectManager;
	Player *m_redTeam;
	Player *m_blueTeam;
};

//=============================================================================
#endif

// EOF
