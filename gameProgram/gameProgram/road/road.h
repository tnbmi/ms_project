//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [road.h]
// Author : KAZUMA OOIGAWA
//*****************************************************************************

#ifndef MY_TEMPLATE_H_
#define MY_TEMPLATE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RoadManager;
class Thread;
class Phase;
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

	static bool Create(Road** outPointer , LPDIRECT3DDEVICE9 device );
	bool Initialize(LPDIRECT3DDEVICE9 device);
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
	 static ROAD_STATE m_roadState;
	 static Phase* m_initialize;
	 static Phase* m_finalize;
};

//=============================================================================
#endif

// EOF
