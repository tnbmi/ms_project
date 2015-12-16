//*****************************************************************************
//
// 準備フェーズ [standby.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_STANDBY_H_
#define MY_STANDBY_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class StandbyImport;

class Shader;
class Camera;
class Light;

class ObjectList;
class UpdateList;
class DrawListManager;
class StandbyMaster;

class Standby : public Phase
{
public:
	Standby(LPDIRECT3DDEVICE9 device);
	~Standby(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	StandbyImport* m_import;

	Shader*	m_shader;
	Camera*	m_camera;
	Light*	m_light;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	StandbyMaster*	 m_standbyMaster;
};

//=============================================================================
#endif

// EOF
