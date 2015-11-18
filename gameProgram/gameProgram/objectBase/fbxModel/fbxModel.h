//*****************************************************************************
//
// FBX���f���N���X [fbxModel.h]
// Author : YUTO HARADA
//
//*****************************************************************************

//=============================================================================
//2015/11/14 ��̑S�Ă̋@�\������
//=============================================================================

#ifndef FBX_MODEL_H_
#define FBX_MODEL_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FbxModel : public ObjectBase
{
public:
	FbxModel(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type );
	virtual ~FbxModel(void);

	static bool Create(FbxModel** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type,char *LoadModelPath);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

	void StartAnimation( const int startKeyFrame,const int endKeyFrame,const bool isRoop );

private:

	inline float Cube( float T ){ return T * T * ( 3.0f - 2.0f * T ); }

	//��ł����܂�
	void TempInit();
	void TempUninit();

	int m_noBone;//int�l�Ƃ��Ďg���������
	

	LPD3DXCONSTANTTABLE m_vs_constant_table;
	LPD3DXCONSTANTTABLE m_ps_constant_table;
	IDirect3DVertexShader9 *m_vertex_shader;
	IDirect3DPixelShader9  *m_pixel_shader;
	LPDIRECT3DVERTEXDECLARATION9 m_decl;

	
	struct MAT
	{
		D3DXVECTOR4 ambent;
		D3DXVECTOR4 diffuse;
		D3DXVECTOR4 specular;
		D3DXVECTOR4 emission;
		float shiniess;
	};

	//Fbx�p���_�\����
	struct FBXVTX
	{
		D3DXVECTOR3 vtx;
		D3DXVECTOR3 nor;
		D3DXVECTOR2 uv;
		D3DXVECTOR4 weight;
		D3DXVECTOR4 boneIdx;
	};

	struct FBXMODELDATA
	{
		MAT mat;
		//�o�b�t�@
		LPDIRECT3DVERTEXBUFFER9 vtxBuff;
		LPDIRECT3DINDEXBUFFER9  idxBuff;
		int vtxSum;
		int idxSum;

		LPDIRECT3DTEXTURE9 tex;
		char *texName;

		//�{�[�����g�������z��ɂ��Q�ƃ{�[�������炷
		int *usingBoneArray;
		int usingBoneSum;
	};

	struct FBXPARTDATA
	{
		FBXMODELDATA *dataArray;
		int dataSum;
	};

	FBXPARTDATA *m_part;
	int m_partSum;

	//�{�[�� �A�j���[�V����
	struct KEYFRAME
	{
		D3DXVECTOR3 trans;
		D3DXQUATERNION rot;
		D3DXVECTOR3 scl;
		float time;
	};

	struct BONEDATA
	{
		KEYFRAME *keyFrameArray;
		int keyMax;
		D3DXMATRIX offsetBoneMtx;
		char boneName[128];
		int boneIdx;//�ǂ����z��ԍ��ł��x�n
		int *childIdxArray;
		int childNum;

		//�A�j���[�V�����s��
		D3DXMATRIX mtxAnim;
		//�e�q�֌W��
		D3DXMATRIX mtxTemp;
		//�ŏI�I�ȍs��
		D3DXMATRIX mtxWorld;
	};

	static const int _usingBoneMax = 50;
	//�u�����h�t���[��
	static const int _blendFrame = 100;

	BONEDATA *m_boneArray;//�{�[��
	int m_boneSum;

	//�A�j���[�V����
	bool m_isAnimRoop;
	int m_startKeyFrame;//�J�n�t���[��
	int m_curKeyFrame;//���݃t���[��
	int m_endKeyFrame;//�I���t���[��
	int m_animTime;

	//�u�����h
	bool m_isBlendRoop;
	int  m_blendStartKeyFrame;
	int  m_blendCurKeyFrame;
	int  m_blendEndKeyFrame;
	int  m_blendTime;
	//�u�����h
	int m_blendWeight;

	void UpdateAnimation();
	//�{�[���̐e�q�֌W��K�p
	void RecursiveBone( BONEDATA *bone,const D3DXMATRIX &mtx );

	//�g�p����{�[���z��
	void SetUpUsingBoneArray( D3DXMATRIX *mtxArray,const int idx1,const int idx2 );

	//���[�h�����
	bool LoadFbxModel( const char *loadModelPath );
};

/*
class FbxMode
{
	private:
	FbxModel *m_fbxModel;
	public:
	FbxMode( FbxModel *model ){ m_fbxModel = model; }
	virtual ~FbxMode(){}
	virtual void Update();
	virtual void Draw();
}
*/

//=============================================================================
#endif

// EOF