//*****************************************************************************
//
// ゲーム時間管理 [timeManager.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_TIME_MANAGER_H_
#define MY_TIME_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ObjectList;
class UpdateList;
class DrawListManager;
class Import;
class Number;

class TimeManager
{
public:
	TimeManager(void);
	~TimeManager(void);

	static bool Create(TimeManager** outPointer,
					   ObjectList* objList,
					   UpdateList* updList,
					   DrawListManager* drwList,
					   LPDIRECT3DDEVICE9 device,
					   Import* import,
					   int time);
	bool Initialize(ObjectList* objList,
					UpdateList* updList,
					DrawListManager* drwList,
					LPDIRECT3DDEVICE9 device,
					Import* import,
					int time);
	void Finalize(void);
	bool Update(void);
	void Draw(void);

	int GetCount(void){return m_count;}

private:
	bool InitObject(void);

	Import*	m_import;
	LPDIRECT3DDEVICE9	m_device;

	ObjectList*			m_objectList;
	UpdateList*			m_updateList;
	DrawListManager*	m_drawListManager;

	int m_time;
	int m_count;
	Number* m_poly_list[2];
};

//=============================================================================
#endif

// EOF
