//*****************************************************************************
//
// エフェクトマネージャ [effectManager.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef EFFECTMANAGER_H_
#define EFFECTMANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"
#include "..\objectBase\instancingBillboard\instancingBillboard.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class ObjectList;
class UpdateList;
class DrawListManager;

class EffectManager
{
public:
	EffectManager(InstancingBillboard *insBill);
	virtual ~EffectManager(void);

	static bool Create(EffectManager** outPointer,LPDIRECT3DDEVICE9 device,ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						const int PolygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
						
	static bool Create(EffectManager** outPointer,InstancingBillboard *insBill );
	bool Initialize(void);
	void Finalize(void);
	void Update(void);

	//補完形式
	typedef enum COMPLETIONMODE
	{
		COMP_MODE_LINE,
		COMP_MODE_EASEIN,
		COMP_MODE_EASEOUT,
		COMP_MODE_CUBE,
		kCompMax

	}COMPLETIONMODE;

	
	typedef struct KEY
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scl;
		D3DXVECTOR4 col;
	};

	typedef struct ANIMATION
	{
		int animSum;
		D3DXVECTOR2 startUvOffset;
	};

	//エフェクト構造体
	typedef struct PARTICLE
	{
		KEY key[2];//キーフレーム制にしようかと思ったけどめんどくさいので2点間の補完でいいや
		int compFrame;
		ANIMATION anim;//アニメーションあるならやるよ
		int animFrame;

		int compErase;
		float animErase;

		float lifeSpan;//今のところ補完フレームと寿命は同じ
		COMPLETIONMODE mode;

		InstancingBillboard::POLYGONDATA *polygon;//描画用
		bool isDelete;
	};

	//作ったエフェクトファイルのデータ読み込み先
	struct EFFECTDATABASE
	{
		PARTICLE *particleArray;
		int particleSum;
	};


	//エフェクトの追加　空きなければ無視される
	//CompFrame 補完フレーム数
	//Life　命　現在は補完フレーム数と同じにしておいてください
	//AnimFrame 総アニメーションフレーム数
	//Offset UvOffset位置
	//CompMode 補完モード
	void AddParticle( const D3DXVECTOR3 &posSt,const D3DXVECTOR3 &sclSt,const D3DXVECTOR4 &colSt,
					const D3DXVECTOR3 &posEd,const D3DXVECTOR3 &sclEd,const D3DXVECTOR4 &colEd,
					const int compFrame,const int life,const int animSum,const int animFrame,const int offsetX,const int offsetY,
					const COMPLETIONMODE compMode );

	void SetDevice( LPDIRECT3DDEVICE9 Device ){ m_device = Device; }

	void LoadTexture( const char *filePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
	void SetTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
	void SetCamPos( const D3DXVECTOR3 &camPos );

	//エフェクトデータを読み込んでデータとして保存する
	void LoadEffectData( const char *filePath );

	//読み込んだデータからエフェクトを呼び出す　Idxはロード順(0から）
	//引数の位置は出現位置
	void AddEffectFromDataBase( const int idx,const D3DXVECTOR3 pos );

	//加算合成等のオプション
	void SetOption( InstancingBillboard::OPTION option );

	void SetViewMtx( const D3DXMATRIX &viewMtx );

	//生成
	void SetEffectGenData( const int genIdx,const int genTime,const int effectIdx,const D3DXVECTOR3 &pos );

private:

	LPDIRECT3DDEVICE9 m_device;

	InstancingBillboard *m_insBill;
	PARTICLE *m_particleArray;
	int m_particleMax;
	int m_cursol;

	//エフェクトデータベース
	const static int kEffectDataBaseMax = 10;
	int m_databaseCursol;//データ入れる先のカーソル
	EFFECTDATABASE m_effectDatabase[ kEffectDataBaseMax ];

	//エフェクト生成シード
	struct EFFECTGENDATA
	{
		int time;//時間
		int genTime;//生成時間
		int effectIdx;
		D3DXVECTOR3 pos;
	};

	const static int _effectGenMax = 5;

	//生成シード
	EFFECTGENDATA m_effectGenArray[ _effectGenMax ];
};

//=============================================================================
#endif

// EOF