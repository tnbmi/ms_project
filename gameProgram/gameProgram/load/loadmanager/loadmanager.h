//*****************************************************************************
//
// ソースのテンプレート [loadmanager.h]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

#ifndef LOAD_MANAGER_H_
#define LOAD_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class LoadImport;
class Shader;
class LoadPolygon;
class Camera;
class Phase;
class LoadManager
{
public:
	enum LOAD_STATE
	{
		LOAD_STATE_CLOSE = 0,
		LOAD_STATE_OPEN,
		LOAD_STATE_NONE
	};
	LoadManager(void);
	virtual ~LoadManager(void);

	static bool Create(LoadManager** outPointer , LPDIRECT3DDEVICE9 device );
	bool Initialize(LPDIRECT3DDEVICE9 device );
	void Finalize(void);
	void Update(void);
	void Draw(void);
	void Draw( Phase* phase );

	void InitializeOpenPos( void );
	void InitializeClosePos( void );
	LOAD_STATE GetState( void ){ return m_state;}
	void SetState( LOAD_STATE state){ m_state = state;}
	void StateOpen( void ){ m_state = LOAD_STATE_OPEN; }
	void StateClose( void ){ m_state = LOAD_STATE_CLOSE; }
	void StateNone( void ){ m_state = LOAD_STATE_NONE; }
	static bool GetCloseFlag( void ){return m_closeFlag;}
	static void SetCloseFlag( bool flag ){ m_closeFlag = flag ;}
private:
	void Open( void );
	void Close( void );

	LPDIRECT3DDEVICE9 m_device;

	Shader*	m_shader;

	LoadImport*	m_import;
	LoadPolygon* m_fadeLeft;//襖or障子の左側
	LoadPolygon* m_fadeRight;//襖or障子の右側
	LoadPolygon* m_icon;//ロード中動かすアイコン的なもの
	LoadPolygon* m_text;//ロード中動かすテキスト的なもの

	float m_textSpeed;//上下に動かすようの

	LOAD_STATE m_state;

	static bool m_closeFlag;
};

//=============================================================================
#endif

// EOF
