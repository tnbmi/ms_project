//*****************************************************************************
//
// �Q�[�����ԊǗ� [timeManager.h]
// Author : KEN MATSUURA
//
//*****************************************************************************

#ifndef MY_TIME_MANAGER_H_
#define MY_TIME_MANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
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

	int GetTime(void){return m_time;}

private:
	bool InitObject(void);

	Import*	m_import;
	LPDIRECT3DDEVICE9	m_device;

	ObjectList*			m_objectList;
	UpdateList*			m_updateList;
	DrawListManager*	m_drawListManager;

	int m_time;
	Number* m_poly_list[2];
};

//=============================================================================
#endif

// EOF
