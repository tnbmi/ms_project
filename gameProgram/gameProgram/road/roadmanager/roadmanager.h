//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [roadmanager.h]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

#ifndef ROAD_MANAGER_H_
#define ROAD_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
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
	static bool GetCloseFlag( void ){return m_closeFlag;}
	static void SetCloseFlag( bool flag ){ m_closeFlag = flag ;}
private:
	void Open( void );
	void Close( void );

	LPDIRECT3DDEVICE9 m_device;

	LPDIRECT3DVERTEXDECLARATION9 m_decl3D;


	Shader*	m_shader;

	RoadImport*	m_import;
	RoadPolygon* m_fadeLeft;//��or��q�̍���
	RoadPolygon* m_fadeRight;//��or��q�̉E��
	RoadPolygon* m_icon;//���[�h���������A�C�R���I�Ȃ���
	RoadPolygon* m_text;//���[�h���������e�L�X�g�I�Ȃ���

	float m_textSpeed;//�㉺�ɓ������悤��

	ROAD_STATE m_state;

	static bool m_closeFlag;
};

//=============================================================================
#endif

// EOF
