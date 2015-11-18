//*****************************************************************************
//
// タイトルフェーズ [title.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TITLE_H_
#define MY_TITLE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"
#include "..\..\commandmanager\commandmanager.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TitleImport;
class Camera;
class ObjectList;
class UpdateList;
class DrawListManager;
class Commandmanager;
class EffectManager;

class Title : public Phase
{
public:
	Title(LPDIRECT3DDEVICE9 device);
	virtual ~Title(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	TitleImport* m_import;
	Camera*		 m_camera;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	Commandmanager*	m_command_manager;
	EffectManager * m_effectManager;

};

//=============================================================================
#endif

// EOF
