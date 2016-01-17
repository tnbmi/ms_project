//*****************************************************************************
//
// ���U���g�̃C���|�[�g [resultImport.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_RESULT_IMPORT_H_
#define MY_RESULT_IMPORT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ResultImport : public Import
{
public:
	enum TEX_TABLE
	{
		// �Ȃ�
		NONE = 0,

		GROUND,
		BG,
		SKY,

		NUM,
		SYOURIRED,
		SYOURIBLUE,
		KEKKA,
		REDOUGI,
		BLUEOUGI,
		REDHIKIWAKE,
		BLUEHIKIWAKE,

		TEX_MAX
	};

	ResultImport(void);
	~ResultImport(void);

	static bool Create(ResultImport** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(LPDIRECT3DDEVICE9 device);
	void Finalize(void);

	LPDIRECT3DTEXTURE9 texture(TEX_TABLE id) {return m_texture[id];}
};

//=============================================================================
#endif

// EOF
