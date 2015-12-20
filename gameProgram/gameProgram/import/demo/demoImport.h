//*****************************************************************************
//
// �f���̃C���|�[�g [demoImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_DEMO_IMPORT_H_
#define MY_DEMO_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DemoImport : public Import
{
public:
	enum TEX_TABLE
	{
		// �Ȃ�
		NONE = 0,

		GROUND,
		BG,
		SKY,

		COMMAND_TEX,
		BLUE_TEX,
		RED_TEX,
		COMMAND_FRAM_00,
		NUMBER_TEX,
		NUMBER_YELLOW,
		NUMBER_WHITE,
		GAME_UI,
		CUTINRED,
		CUTINBLUE,
		COUNTDOWN,
		EFFECT,

		DEMO_LOGO,

		TEX_MAX
	};

	DemoImport(void);
	~DemoImport(void);

	static bool Create(DemoImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
