//*****************************************************************************
//
// デモフェーズ [demo.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_DEMO_H_
#define MY_DEMO_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DemoImport;

class Shader;
class Camera;
class Light;

class ObjectList;
class UpdateList;
class DrawListManager;

class Player;
class DemoMaster;

class Demo : public Phase
{
public:
	Demo(LPDIRECT3DDEVICE9 device);
	~Demo(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	DemoImport*	m_import;

	Shader*	m_shader;
	Camera*	m_camera;
	Light*	m_light;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	DemoMaster *m_demoMaster;
};

//=============================================================================
#endif

// EOF
