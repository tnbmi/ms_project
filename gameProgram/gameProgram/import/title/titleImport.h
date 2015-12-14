//*****************************************************************************
//
// �^�C�g���̃C���|�[�g [titleImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_TITLE_IMPORT_H_
#define MY_TITLE_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class TitleImport : public Import
{
public:
	enum TEX_TABLE
	{
		// �Ȃ�
		NONE = 0,

		TITLE_LOGO,
		SKY,

		TEX_MAX
	};

	TitleImport(void);
	~TitleImport(void);

	static bool Create(TitleImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
