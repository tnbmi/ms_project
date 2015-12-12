//*****************************************************************************
//
// ソースのテンプレート [roadpolygon.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "roadpolygon.h"

//=============================================================================
// コンストラクタ
//=============================================================================
RoadPolygon::RoadPolygon(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_tex =NULL;
	m_vtxBuff = NULL;
	m_pos =D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rot = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	float m_fLength=0.0f;
	float m_fAngle=0.0f;
	m_diffuse = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
}

//=============================================================================
// デストラクタ
//=============================================================================
RoadPolygon::~RoadPolygon(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool RoadPolygon::Create(RoadPolygon** outPointer , LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize , POLYGON_CRITERIA crit )
{
	RoadPolygon* pointer = new RoadPolygon();
	if(!pointer->Initialize(device , texture , polygonSize , crit))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool RoadPolygon::Initialize( LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize ,POLYGON_CRITERIA crit )
{
	//----------------------------
	// コメント
	//----------------------------
	m_tex = texture;
	m_device = device;
	m_scl = D3DXVECTOR3( polygonSize.x , polygonSize.y , 0.0f );
	SetVertex( crit );
	m_crit = crit;
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void RoadPolygon::Finalize(void)
{
	if( m_tex != NULL )
	{
		m_tex = NULL;
	}
	if( m_vtxBuff != NULL )
	{
		m_vtxBuff ->Release();
		m_vtxBuff =NULL;
	}
	delete this;
}

//=============================================================================
// 更新
//=============================================================================
void RoadPolygon::Update(void)
{
	//SetVertex( m_crit );
}

//=============================================================================
// 描画
//=============================================================================
void RoadPolygon::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc)
{
	//----------------------------
	// ワールド行列計算
	//----------------------------
	D3DXMATRIX wvp;
	D3DXMATRIX world, scaling, rotation, translate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&world);
	// スケールの反映
	D3DXMatrixScaling(&scaling, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&world, &world, &scaling);
	// 回転の反映
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&world, &world, &rotation);
	// 位置の反映
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&world, &world, &translate);

	//----------------------------
	// ワールドデータ設定
	//----------------------------
	// バーテックス
	
	if(vsc != nullptr)
		// ワールドマトリックスの設定
		vsc->SetMatrix(m_device, "gWorld", &world);

	//----------------------------
	// サンプラー準備
	//----------------------------
	unsigned int texSumpler = -1;
	if(psc != nullptr)
	{
		// サンプラーインデックス
		texSumpler = psc->GetSamplerIndex("texSampler");

		// サンプラーステートパラメータ
		m_device->SetSamplerState(texSumpler, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);	// テクスチャアドレッシング方法(U値)を設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);	// テクスチャアドレッシング方法(V値)を設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小フィルタモードを設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大フィルタモードを設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAXANISOTROPY, 16);
	}

	//----------------------------
	// テクスチャ設定
	//----------------------------
	if(texSumpler >= 0)
		//テクスチャの設定
		m_device->SetTexture(texSumpler, m_tex);
	else
		m_device->SetTexture(0, m_tex);

	//----------------------------
	// 描画
	//----------------------------
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
								2,
								m_vtx,
								sizeof(VERTEX));
}
//=============================================================================
// ポリゴン設定
//=============================================================================
void RoadPolygon::SetVertex( POLYGON_CRITERIA crit )
{
	// 座標
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x, -half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x,  half.y, half.z);

	// 頂点カラー
	m_vtx[0].color = m_diffuse;
	m_vtx[1].color = m_diffuse;
	m_vtx[2].color = m_diffuse;
	m_vtx[3].color = m_diffuse;

	// テクスチャ座標
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// 頂点色変換
//=============================================================================
void RoadPolygon::SetVertexColor(void)
{
	// 頂点カラー
	m_vtx[0].color = m_diffuse;
	m_vtx[1].color = m_diffuse;
	m_vtx[2].color = m_diffuse;
	m_vtx[3].color = m_diffuse;
}

// EOF
