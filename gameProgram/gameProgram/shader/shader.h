//*****************************************************************************
//
// �V�F�[�_�[�Ǘ� [shader.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_SHADER_H_
#define MY_SHADER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Shader
{
public:
	enum PATTERN
	{
		PAT_NONE = 0,
		PAT_LIGHT,
		PAT_FBX,
		PAT_INS,
		PAT_2D,

		PATTERN_MAX
	};

	Shader(LPDIRECT3DDEVICE9 device);
	~Shader(void);

	static bool Create(Shader** outPointer, LPDIRECT3DDEVICE9 device);
	bool Initialize(void);
	void Finalize(void);

	void SetShader(LPD3DXCONSTANTTABLE* outVSC, LPD3DXCONSTANTTABLE* outPSC, PATTERN patternId);

	LPD3DXCONSTANTTABLE vsc(int id){return m_vsc[id];}
	LPD3DXCONSTANTTABLE psc(int id){return m_psc[id];}

private:
	LPDIRECT3DDEVICE9 m_device;

	int m_vsSize;
	LPDIRECT3DVERTEXSHADER9* m_vertexShader;	// �o�[�e�b�N�X�V�F�[�_�[
	LPD3DXCONSTANTTABLE*	 m_vsc;				// �o�[�e�b�N�X�V�F�[�_�[�R���X�^���g�e�[�u��

	int m_psSize;
	LPDIRECT3DPIXELSHADER9*	m_pixelShader;		// �s�N�Z���V�F�[�_�[
	LPD3DXCONSTANTTABLE*	m_psc;				// �s�N�Z���V�F�[�_�[�R���X�^���g�e�[�u��
};

//=============================================================================
#endif

// EOF
