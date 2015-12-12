//*****************************************************************************
//
// ソースのテンプレート [template.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TEMPLATE_H_
#define MY_TEMPLATE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class ObjectList;
class UpdateList;
class DrawListManager;

class Score;
class Player;
class FbxModel;
class EffectManager;
class ResultImport;
class Debugproc;
class PadXManager;

class ResultMaster
{
public:

	ResultMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,ResultImport *import,Debugproc *proc,PadXManager* padXMaster ); 

	~ResultMaster(void);

	static bool Create(ResultMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						ResultImport* import,Debugproc* debugproc,PadXManager* padXManager);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:

	enum PHASE
	{
		PHASE_RESULTSTART,
		PHASE_ANNOUNCEMENT,
		PHASE_ANNOUNCEFINISH,
		PHASE_MAX
	};

	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	ResultImport*	 m_import;
	Debugproc*		 m_debugProc;
	PadXManager*	 m_padXManager;

	PHASE			m_phase;
	Score*			m_redTeamScore;
	Score*			m_blueTeamScore;
	Player*			m_redTeam;
	Player*			m_blueTeam;
	FbxModel*		m_redGgy;
	FbxModel*		m_blueGgy;
	EffectManager*	m_effectManager;
	int m_fireTime;
	static const int _shotFrame = 200;
};

//=============================================================================
#endif

// EOF
