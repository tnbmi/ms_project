//*****************************************************************************
//
// ���U���g�t�F�[�Y [result.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_RESULT_H_
#define MY_RESULT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ResultImport;
class Shader;
class Camera;
class Light;

class ObjectList;
class UpdateList;
class DrawListManager;

class ResultMaster;

class Result : public Phase
{
public:
	Result(LPDIRECT3DDEVICE9 device);
	~Result(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	Shader*	m_shader;
	Light*	m_light;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;

	ResultImport*	m_import;
	Camera*			m_camera;

	ResultMaster*	m_resultMaster;
};

//=============================================================================
#endif

// EOF
