//*****************************************************************************
//
// 待機画面 [standbyMaster.h]
// Author : YUTO HARAFA
//
//*****************************************************************************

#ifndef STANDBY_MASTER_H_
#define STANDBY_MASTER_H_
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
class StandbyImport;
class Debugproc;
class PadXManager;
class Light;
class Polygon2D;
class Ggy2DAnimationManager;

class StandbyMaster
{
public:

	StandbyMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,StandbyImport *import,Debugproc *proc,PadXManager* padXMaster,Light *light ); 

	~StandbyMaster(void);

	static bool Create(StandbyMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						StandbyImport* import,Debugproc* debugproc,PadXManager* padXManager,Light *light);
	bool Initialize(void);
	void Finalize(void);
	bool Update(void);
	void Draw(void);

private:

	enum PHASE
	{
		PHASE_WAIT,
		PHASE_STANDBY,
		PHASE_NEXTSCENE
	};


	struct STANDBYDATA
	{
		Polygon2D* pol;
		bool isStandby;
		bool isPrevStandby;
		float time;//補完用
	};

	struct ANIMPOLDATA
	{
		Polygon2D *pol;
		float time;
		D3DXVECTOR2 uv[4];
		D3DXVECTOR2 offsetUv;
		int animSum;
	};

	static const int _compFrame = 30;
	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	StandbyImport*	 m_import;
	Debugproc*		 m_debugProc;
	PadXManager*	 m_padXManager;
	Light*	m_light;
	Polygon2D* m_back;
	Polygon2D* m_mid;
	Polygon2D* m_front;
	Polygon2D* m_battle;

	float m_compTime;

	STANDBYDATA m_blueTeamStandby[2];
	STANDBYDATA m_redTeamStandby[2];

	PHASE m_phase;
	ANIMPOLDATA m_redLogo;
	ANIMPOLDATA m_blueLogo;
	Ggy2DAnimationManager *m_ggyAnimManager;

};

//=============================================================================
#endif

// EOF