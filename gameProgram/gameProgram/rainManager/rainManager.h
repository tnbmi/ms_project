//*****************************************************************************
//
// 何かを降らせる [rainManager.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef RAINMANAGER_H_
#define RAINMANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"
#include "..\objectBase\instancingBillboard\instancingBillboard.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class InstancingBillboard;
class ObjectList;
class UpdateList;
class DrawListManager;

class RainManager
{
public:
	RainManager(void);
	~RainManager(void);

	static bool Create(RainManager** outPointer,LPDIRECT3DDEVICE9 device,ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						const int PolygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize);
	bool Initialize(InstancingBillboard *bill);
	void Finalize(void);
	void Update(void);

	void SetRainSeed( const int idx,const float createFrame,
									const int createSumMin,const int createSumMax,
									const float massMin,const float massMax,
									const D3DXVECTOR3 &forceMin,const D3DXVECTOR3 &forceMax,
									const D3DXVECTOR3 &setPosMin,const D3DXVECTOR3 &setPosMax );

private:

	struct RAIN_DATA
	{
		float mass;//重さ
		D3DXVECTOR3 force;//力
		D3DXVECTOR3 acc;//加速度
		D3DXVECTOR3 spd;//速度
		D3DXVECTOR3 pos;
		InstancingBillboard::POLYGONDATA *poly;
		bool isDelete;
	};

	struct SEED_DATA
	{
		float mass[2];
		D3DXVECTOR3 force[2];
		D3DXVECTOR3 setPos[2];
		float createFrame;
		float frame;
		int createSum[2];
		bool isUse;
	};

	RAIN_DATA *m_rainDataArray;

	static const int _seedMax = 5;
	SEED_DATA m_seedDataArray[_seedMax];
	int m_seedCursol;

	//使用可能なデータidxを返す
	int LentRainData();

	LPDIRECT3DDEVICE9 m_device;
	InstancingBillboard *m_insBill;
	int m_particleMax;
	float m_gravity;//重力値
	D3DXVECTOR3 m_wind;//風
	int m_cursol;
};

//=============================================================================
#endif

// EOF
