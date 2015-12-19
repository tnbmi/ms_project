//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [loadmanager.h]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

#ifndef LOAD_MANAGER_H_
#define LOAD_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
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
	LoadPolygon* m_fadeLeft;//��or��q�̍���
	LoadPolygon* m_fadeRight;//��or��q�̉E��
	LoadPolygon* m_icon;//���[�h���������A�C�R���I�Ȃ���
	LoadPolygon* m_text;//���[�h���������e�L�X�g�I�Ȃ���

	float m_textSpeed;//�㉺�ɓ������悤��

	LOAD_STATE m_state;

	static bool m_closeFlag;
};

//=============================================================================
#endif

// EOF
