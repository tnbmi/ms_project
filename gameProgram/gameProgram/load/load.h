//*****************************************************************************
//
// ソースのテンプレート [load.h]
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
class LoadManager;
class Thread;

class Phase;
class MainImport;
class FbxTexImport;

class Load
{
public:
	struct START_LOAD
	{
		Phase*			phase;
		MainImport**	mainImport;
		FbxTexImport**	fbxTexImport;
	};

	enum LOAD_STATE
	{
		LOAD_STATE_PRIM = 0,
		LOAD_STATE_FINALIZE_START,
		LOAD_STATE_FINALIZE_END,
		LOAD_STATE_INITIALIZE_START,
		LOAD_STATE_INITIALIZE_END,
		LOAD_STATE_COMPLETE
	};

	Load(void);
	virtual ~Load(void);

	static bool Create(Load** outPointer , LPDIRECT3DDEVICE9 device );
	bool Initialize(LPDIRECT3DDEVICE9 device );
	void Finalize(void);

	void StartLoading( START_LOAD* startLoad );
	void Loading( Phase* finalize , Phase* initialize );
	static void StateOpen( void );
	static void StateClose( void );

	static void SetUp( void );
	static void Change( void );

private:
	Thread* m_thread;

	static LPDIRECT3DDEVICE9 m_device;
	static LoadManager*		 m_loadManager;

	static bool m_loadingFlag;
	static bool m_primUpdate;
	static bool m_closeFlag;
	static LOAD_STATE m_loadState;

	static Phase* m_initialize;
	static Phase* m_finalize;

	static START_LOAD* m_startLoad;
};

//=============================================================================
#endif

// EOF
