//*****************************************************************************
//
// �^�C�g���t�F�[�Y [title.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TITLE_H_
#define MY_TITLE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TitleImport;

class Shader;
class Camera;
class Light;

class ObjectList;
class UpdateList;
class DrawListManager;
class Tex2DAnimation;
class FbxModel;

class Title : public Phase
{
public:
	Title(LPDIRECT3DDEVICE9 device);
	~Title(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	TitleImport* m_import;

	Shader*	m_shader;
	Camera*	m_camera;
	Light*	m_light;

	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	Tex2DAnimation * m_pushLogo;

	//
	struct TitleAnimTable
	{
		int nebSt,nebEd;
		int ggySt,ggyEd;
	};

	TitleAnimTable m_animTable[8];

	//neb
	FbxModel *m_nebRed;
	FbxModel *m_nebBlue;
	int m_animIdx;
};

//=============================================================================
#endif

// EOF
