//*****************************************************************************
//
// �����̃C���|�[�g [standbyImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_STANDBY_IMPORT_H_
#define MY_STANDBY_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class StandbyImport : public Import
{
public:
	enum TEX_TABLE
	{
		// �Ȃ�
		NONE = 0,

		BACK,
		MID,
		FRONT,
		PREPARE_BLUE,
		PREPARE_RED,
		BATTLE,
		SELECT_BLUE,
		SELECT_RED,

		TEX_MAX
	};

	StandbyImport(void);
	~StandbyImport(void);

	static bool Create(StandbyImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
