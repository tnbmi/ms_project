//*****************************************************************************
//
// FBXモデルクラス [fbxModel.h]
// Author : YUTO HARADA
//
//*****************************************************************************

//=============================================================================
//2015/11/14 大体全ての機能を実装
//=============================================================================

#ifndef FBX_MODEL_H_
#define FBX_MODEL_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
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

	int m_noBone;//int値として使いたいんで
	
	LPDIRECT3DVERTEXDECLARATION9 m_decl;

	
	struct MAT
	{
		D3DXVECTOR4 ambent;
		D3DXVECTOR4 diffuse;
		D3DXVECTOR4 specular;
		D3DXVECTOR4 emission;
		float shiniess;
	};

	//Fbx用頂点構造体
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
		//バッファ
		LPDIRECT3DVERTEXBUFFER9 vtxBuff;
		LPDIRECT3DINDEXBUFFER9  idxBuff;
		int vtxSum;
		int idxSum;

		LPDIRECT3DTEXTURE9 tex;
		char *texName;

		//ボーンを使うだけ配列にし参照ボーンを減らす
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

	//ボーン アニメーション
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
		int boneIdx;//どうせ配列番号ですＹＯ
		int *childIdxArray;
		int childNum;

		//アニメーション行列
		D3DXMATRIX mtxAnim;
		//親子関係込
		D3DXMATRIX mtxTemp;
		//最終的な行列
		D3DXMATRIX mtxWorld;
	};

	static const int _usingBoneMax = 50;
	//ブレンドフレーム
	static const int _blendFrame = 100;

	BONEDATA *m_boneArray;//ボーン
	int m_boneSum;

	//アニメーション
	bool m_isAnimRoop;
	int m_startKeyFrame;//開始フレーム
	int m_curKeyFrame;//現在フレーム
	int m_endKeyFrame;//終了フレーム
	int m_animTime;

	//ブレンド
	bool m_isBlendRoop;
	int  m_blendStartKeyFrame;
	int  m_blendCurKeyFrame;
	int  m_blendEndKeyFrame;
	int  m_blendTime;
	//ブレンド
	int m_blendWeight;

	void UpdateAnimation();
	//ボーンの親子関係を適用
	void RecursiveBone( BONEDATA *bone,const D3DXMATRIX &mtx );

	//使用するボーン配列
	void SetUpUsingBoneArray( D3DXMATRIX *mtxArray,const int idx1,const int idx2 );

	//ロードするんご
	bool LoadFbxModel( const char *loadModelPath );
};

//=============================================================================
#endif

// EOF