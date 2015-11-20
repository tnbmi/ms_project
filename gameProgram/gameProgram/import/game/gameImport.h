//*****************************************************************************
//
// �Q�[���̃C���|�[�g [gameImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_GAME_IMPORT_H_
#define MY_GAME_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class GameImport : public Import
{
public:
	enum TEX_TABLE
	{
		// �Ȃ�
		NONE = 0,

		TEST_0,
		TEST_1,

		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_RIGHT,
		BUTTON_LEFT,

		TEX_MAX
	};

	GameImport(void);
	~GameImport(void);

	static bool Create(GameImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
