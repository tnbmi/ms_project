//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [template.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TEMPLATE_H_
#define MY_TEMPLATE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class ObjectList;
class UpdateList;
class DrawListManager;

class Score;
class Player;
class FbxModel;
class EffectManager;
class ResultImport;
class Debugproc;
class PadXManager;
class Light;
class Polygon2D;
class FbxTexImport;
class RainManager;

class ResultMaster
{
public:

	ResultMaster( LPDIRECT3DDEVICE9 device,ObjectList *objectList,UpdateList *updateList,DrawListManager *drawList,ResultImport *import,FbxTexImport *fbxTexImport,Debugproc *proc,PadXManager* padXMaster,Light *light ); 

	~ResultMaster(void);

	static bool Create(ResultMaster** outPointer,LPDIRECT3DDEVICE9 device,
						ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						ResultImport* import,FbxTexImport *fbxTexImport,Debugproc* debugproc,PadXManager* padXManager,Light *light);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:

	enum PHASE
	{
		PHASE_RESULTSTART,
		PHASE_ANNOUNCEMENT,
		PHASE_ANNOUNCEFINISH,
		PHASE_MAX
	};

	LPDIRECT3DDEVICE9 m_device;
	ObjectList*		 m_objectList;
	UpdateList*		 m_updateList;
	DrawListManager* m_drawListManager;
	ResultImport*	 m_import;
	Debugproc*		 m_debugProc;
	PadXManager*	 m_padXManager;
	FbxTexImport*	 m_fbxTexImport;
	Light*	m_light;

	PHASE			m_phase;
	Score*			m_redTeamScore;
	Score*			m_blueTeamScore;
	Player*			m_redTeam;
	Player*			m_blueTeam;
	FbxModel*		m_redGgy;
	FbxModel*		m_blueGgy;
	EffectManager*	m_effectManager;
	RainManager*	m_rainManager;

	//�|���S��
	Polygon2D*	m_resultPoly;//���ʔ��\�̃|���S��
	Polygon2D*	m_winPoly;//�����\���|���S��


	int m_fireTime;
	static const int _shotFrame = 200;
};

//=============================================================================
#endif

// EOF
