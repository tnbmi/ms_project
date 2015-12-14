//*****************************************************************************
//
// ソースのテンプレート [roadmanager.h]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

#ifndef ROAD_MANAGER_H_
#define ROAD_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RoadImport;
class Shader;
class RoadPolygon;
class Camera;
class Phase;
class RoadManager
{
public:
	enum ROAD_STATE
	{
		ROAD_STATE_CLOSE = 0,
		ROAD_STATE_OPEN,
		ROAD_STATE_NONE
	};
	RoadManager(void);
	virtual ~RoadManager(void);

	static bool Create(RoadManager** outPointer , LPDIRECT3DDEVICE9 device );
	bool Initialize(LPDIRECT3DDEVICE9 device );
	void Finalize(void);
	void Update(void);
	void Draw(void);
	void Draw( Phase* phase );

	void InitializeOpenPos( void );
	void InitializeClosePos( void );
	ROAD_STATE GetState( void ){ return m_state;}
	void SetState( ROAD_STATE state){ m_state = state;}
	void StateOpen( void ){ m_state = ROAD_STATE_OPEN; }
	void StateClose( void ){ m_state = ROAD_STATE_CLOSE; }
	void StateNone( void ){ m_state = ROAD_STATE_NONE; }
private:
	void Open( void );
	void Close( void );

	LPDIRECT3DDEVICE9 m_device;

	LPDIRECT3DVERTEXDECLARATION9 m_decl3D;


	Shader*	m_shader;

	RoadImport*	m_import;
	RoadPolygon* m_fadeLeft;//襖or障子の左側
	RoadPolygon* m_fadeRight;//襖or障子の右側
	RoadPolygon* m_icon;//ロード中動かすアイコン的なもの
	RoadPolygon* m_text;//ロード中動かすテキスト的なもの

	float m_textSpeed;//上下に動かすようの

	ROAD_STATE m_state;


};

//=============================================================================
#endif

// EOF
