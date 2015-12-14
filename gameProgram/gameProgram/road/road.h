//*****************************************************************************
//
// ソースのテンプレート [road.h]
// Author : KAZUMA OOIGAWA
//*****************************************************************************

#ifndef MY_TEMPLATE_H_
#define MY_TEMPLATE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RoadManager;
class Thread;
class Phase;
class Manager;
class Road
{
public:
	enum ROAD_STATE
	{
		ROAD_STATE_PRIM = 0,
		ROAD_STATE_FINALIZE_START,
		ROAD_STATE_FINALIZE_END,
		ROAD_STATE_INITIALIZE_START,
		ROAD_STATE_INITIALIZE_END,
		ROAD_STATE_COMPLETE
	};

	Road(void);
	virtual ~Road(void);

	static bool Create(Road** outPointer , LPDIRECT3DDEVICE9 device , Manager* manager );
	bool Initialize(LPDIRECT3DDEVICE9 device , Manager* manager);
	void Finalize(void);

	void Roading( Phase* initialize );
	void Roading( Phase* finalize , Phase* initialize );
	static void StateOpen( void );
	static void StateClose( void );

	static void Change( void );
	static void Change1( Phase *Initialize );
	static void Change2( Phase* finalize , Phase *Initialize );

private:
	 Thread* m_thread;
	 static RoadManager *m_roadManager;
	 static bool m_roadingFlag;
	 static bool m_primUpdate;
	 static ROAD_STATE m_roadState;
	 static Phase* m_initialize;
	 static Phase* m_finalize;
	 static Manager* m_manager;
};

//=============================================================================
#endif

// EOF
