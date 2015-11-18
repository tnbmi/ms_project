//*****************************************************************************
//
// インスタンシングビルボードクラス [instancingBillboard.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef INSTANCING_BILLBOARD_H_
#define INSTANCING_BILLBOARD_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class InstancingBillboard : public ObjectBase
{
public:
	InstancingBillboard(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type );
	virtual ~InstancingBillboard(void);

	static bool Create(InstancingBillboard** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type );
	static bool Create(InstancingBillboard** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type,const int polygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2&oneSize );
	bool Initialize(void);
	bool Initialize( const int polygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

	//インスタンシング情報に使う構造体
	typedef struct POLYGONDATA
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 scl;
		D3DXCOLOR col;
		D3DXVECTOR2 uvOffset;
		float camRange;//カメラとの距離ソートに使う
		bool isDelete;
	};

	//加算合成とか
	typedef struct OPTION
	{
		bool is_additiveSynthesis;//加算合成フラグ
		bool is_zsort;
		bool is_zwrite;

		OPTION(){}
		OPTION( bool isAdd,bool isZSort,bool isZWrite){ is_additiveSynthesis = isAdd;is_zsort = isZSort;is_zwrite = isZWrite; }
	};

		void SetOption( const OPTION opt ){ m_option = opt; }

		//配列からインスタンシング用情報をポインタで貸し出す
		//PolygonInfoを弄ることでポリゴンを描画される
		POLYGONDATA *LentInsPolygonData();

		//テクスチャ再ロード
		void LoadTexture( const char *FilePath );

		//Uvを再設定する　バッファをLockするので注意
		void SetVtxBuffTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 oneSize );


		void SetCameraPos( const D3DXVECTOR3 camPos ){ m_cameraPos = camPos; }
		int GetPolygonMax(){ return m_polygonMax; }

		void SetViewMtx( const D3DXMATRIX &viewMtx ){ m_viewMtx = viewMtx; }

private:

	LPDIRECT3DVERTEXDECLARATION9 m_decl;

	//ビュー行列
	D3DXMATRIX m_viewMtx;

	//バッファ
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;//板ポリ用
	LPDIRECT3DVERTEXBUFFER9 m_instancingBuff;//インスタンシング用
	LPDIRECT3DINDEXBUFFER9 m_idxBuff;//インスタンシングに必須なので仕方なく
		

	int m_polygonMax;//インスタンシングで表示できる最大数
	int m_usingPolygonSum;//現在表示しているポリゴン数
	int m_vtxSize;//頂点形式のサイズ

	//テクスチャ
	D3DXVECTOR2 m_texSize;//テクスチャ自体のサイズ
	D3DXVECTOR2 m_oneSize;//テクスチャシートの切り取りサイズ

	typedef struct VTXBILL
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

	//インスタンシングバッファの形式(カラーがD3DXVECTOR4なのはなぜかD3DXCOLORだとダメだったから）
	typedef struct INSTANCINGDATA
	{
		D3DXMATRIX mtx;
		D3DXVECTOR2 uv_offset;
		D3DXVECTOR4 col;
	};

	
	//インスタンシング用情報配列
	POLYGONDATA **m_insArray;
	int m_cursol;//配列の貸出カーソル　貸し出した位置を保存してその次から検索をすれば多少は検索コストが削減できるはず

	//Option
	OPTION m_option;

	//カメラ位置
	D3DXVECTOR3 m_cameraPos;

	//インスタンシング用バッファに変更を適用する
	void ApplyInstancingBuff();

	//各対応シェーダーの読み込み
	void LoadShaderBill();

	//各頂点バッファの作成
	bool CreateVtxBuffBill();

	//インスタンシングバッファ作成
	bool CreateInstancingBuff();

	//インデックスバッファの作成
	//一見いらないように見えるが、インスタンシングがDrawIndexedPrimitiveのみ対応なのでしゃーなし
	bool CreateIdxBuff();

	//加算合成などのオプションを適応する
	void ApplyOption();

};

//=============================================================================
#endif

// EOF