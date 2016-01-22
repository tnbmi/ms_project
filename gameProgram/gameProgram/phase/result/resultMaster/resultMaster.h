//*****************************************************************************
//
// ソースのテンプレート [template.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TEMPLATE_H_
#define MY_TEMPLATE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
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
		PHASE_FANROT,
		PHASE_MODELIN,
		PHASE_MODELROT,
		PHASE_MODELPOSE,
		PHASE_FIREWORKSANDRAIN,
		PHASE_FINISH,
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
	EffectManager*	m_effectManager;
	RainManager*	m_rainManager;

	//ポリゴン
	Polygon2D*	m_resultPoly;//結果発表のポリゴン
	Polygon2D*	m_winPoly;//勝ち表示ポリゴン

	//新リザルト用
	//ねぶた赤青
	FbxModel *m_redNeb;
	FbxModel *m_blueNeb;

	D3DXVECTOR3 m_redNebPos;
	D3DXVECTOR3 m_blueNebPos;
	D3DXVECTOR3 m_redGgyPos;
	D3DXVECTOR3 m_blueGgyPos;
	float m_redGgyRot;
	float m_blueGgyRot;
	int m_time;

	//背景モデル
	FbxModel *m_back;

	//じじいは↑で宣言済み
	FbxModel *m_redGgy;
	FbxModel *m_blueGgy;

	//扇二種
	Polygon2D *m_fanLeft;
	Polygon2D *m_fanRight;
	float m_fanLeftRot;
	float m_fanRightRot;

	void WinRedTeam();
	void WinBlueTeam();
	void DrawTeam();
};

//=============================================================================
#endif

// EOF
