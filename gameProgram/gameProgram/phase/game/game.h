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

class Shader;
class Camera;
class Light;

class ObjectList;
class UpdateList;
class DrawListManager;

class CommandManager;
class TimeManager;
class Player;
class GameMaster;

class Game : public Phase
{
public:
	Game(LPDIRECT3DDEVICE9 device);
	~Game(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	GameImport*	m_import;

	Shader*	m_shader;
	Camera*	m_camera;
	Light*	m_light;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	CommandManager*	m_command_manager;
	TimeManager*	m_time_manager;

	GameMaster *m_gameMaster;
};

//=============================================================================
#endif

// EOF
