//*****************************************************************************
//
// ソースのテンプレート [loadpolygon.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "loadpolygon.h"
#include "..\..\common\safe.h"

//=============================================================================
// コンストラクタ
//=============================================================================
LoadPolygon::LoadPolygon(void)
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
	m_sheetNum = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
LoadPolygon::~LoadPolygon(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool LoadPolygon::Create(LoadPolygon** outPointer , LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize )
{
	LoadPolygon* pointer = new LoadPolygon();
	if(!pointer->Initialize(device , texture , polygonSize))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool LoadPolygon::Initialize( LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize )
{
	//----------------------------
	// コメント
	//----------------------------
	m_tex = texture;
	m_device = device;
	m_scl = D3DXVECTOR3( polygonSize.x , polygonSize.y , 0.0f );
	SetVertex();
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void LoadPolygon::Finalize(void)
{
	if( m_tex != NULL )
		m_tex = NULL;
	SafeRelease( m_vtxBuff );
}

//=============================================================================
// 更新
//=============================================================================
void LoadPolygon::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void LoadPolygon::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc)
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
void LoadPolygon::SetVertex( void )
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
void LoadPolygon::SetVertexColor(void)
{
	// 頂点カラー
	m_vtx[0].color = m_diffuse;
	m_vtx[1].color = m_diffuse;
	m_vtx[2].color = m_diffuse;
	m_vtx[3].color = m_diffuse;
}
//=============================================================================
// 頂点UV
//=============================================================================
void LoadPolygon::SetUvX( float dividVol , int useSheet )
{
	// テクスチャ座標
	m_vtx[0].uv = D3DXVECTOR2((1.0f*dividVol)*useSheet, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2((1.0f*dividVol)*useSheet+dividVol, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2((1.0f*dividVol)*useSheet, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2((1.0f*dividVol)*useSheet+dividVol, 1.0f);
}
void LoadPolygon::SetUvY( float dividVol , int useSheet )
{
	// テクスチャ座標
	m_vtx[0].uv = D3DXVECTOR2(0.0f, (1.0f*dividVol)*useSheet);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, (1.0f*dividVol)*useSheet);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, (1.0f*dividVol)*useSheet+dividVol);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, (1.0f*dividVol)*useSheet+dividVol);
}
// EOF
