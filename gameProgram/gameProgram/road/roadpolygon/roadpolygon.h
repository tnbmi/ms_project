//*****************************************************************************
//
// スレッド用ポリゴン [roadpolygon.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef ROAD_POLYGON_H_
#define ROAD_POLYGON_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RoadPolygon
{
public:
	typedef enum
	{
		POLYGON_CRITERIA_LEFT = 0,
		POLYGON_CRITERIA_CENTER,
		POLYGON_CRITERIA_RIGHT
	}POLYGON_CRITERIA;
	RoadPolygon(void);
	virtual ~RoadPolygon(void);

	static bool Create(RoadPolygon** outPointer, LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 textue , D3DXVECTOR3 polygonSize , POLYGON_CRITERIA crit );
	bool Initialize(LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize , POLYGON_CRITERIA crit );
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc);
	D3DXVECTOR3 GetPosition( void ){return m_pos;}
	void SetPosition( float x, float y , float z){m_pos.x = x , m_pos.y = y , m_pos.z = z;}
	void SetPosition( D3DXVECTOR3 pos ){ m_pos = pos; }

	D3DXVECTOR3 GetRot( void ){ return m_rot; }
	void SetRot( D3DXVECTOR3 rot ){ m_rot = rot; }
	void SetRot( float x, float y , float z ){ m_rot.x = x , m_rot.y = y , m_rot.z = z; }
	void SetUvX( float dividVol , int useSheet );//(分割数(少数点以下),使用するテクスチャは分割した何枚目か)
	void SetUvY( float dividVol , int useSheet );//(分割数(少数点以下),使用するテクスチャは分割した何枚目か)
	D3DXCOLOR GetColor( void ){ return m_diffuse; }
	void SetColor( D3DCOLOR diff){m_diffuse = diff;SetVertexColor();}
	void SetColor( float r , float g , float b , float a ){m_diffuse = D3DXCOLOR( r,g,b,a );SetVertexColor();}
	float GetSheetNum( void ){return m_sheetNum;}
	void SetSheetNum( float Num ){ m_sheetNum = Num; }
private:
	struct VERTEX
	{
		D3DXVECTOR3	position;
		D3DCOLOR	color;
		D3DXVECTOR2	uv;
	};
	VERTEX m_vtx[4];
	float m_sheetNum;
	LPDIRECT3DDEVICE9	m_device;		// Deviceオブジェクト(描画に必要)
	LPDIRECT3DTEXTURE9	m_tex;			//テクスチャへのポインタ
	D3DXVECTOR3 m_scl;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_pos;
	D3DXCOLOR m_diffuse;
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;	//頂点バッファへのポインタ
	POLYGON_CRITERIA m_crit;

	void SetVertex( POLYGON_CRITERIA crit );
	void SetVertexColor(void);
};

//=============================================================================
#endif

// EOF
