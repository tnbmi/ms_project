//*****************************************************************************
//
// 3Dポリゴンの基盤 [polygon3D.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "polygon3D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Polygon3D::Polygon3D(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : ObjectBase(device, objectList, type)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_scl = D3DXVECTOR3(64.0f, 64.0f, 0.0f);

	m_norTexture = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
Polygon3D::~Polygon3D(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Polygon3D::Create(Polygon3D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,
					LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type)
{
	Polygon3D* pointer = new Polygon3D(device, objectList, type);
	if(!pointer->Initialize(texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Polygon3D::Initialize(LPDIRECT3DTEXTURE9 texture)
{
	//----------------------------
	// 頂点情報設定
	//----------------------------
	SetVertex();

	//----------------------------
	// テクスチャ設定
	//----------------------------
	m_texture = texture;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Polygon3D::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void Polygon3D::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void Polygon3D::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	//----------------------------
	// ワールド行列計算
	//----------------------------
	D3DXMATRIX wvp;
	D3DXMATRIX scaling, rotation, translate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_world);
	// 回転の反映
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_world, &m_world, &rotation);
	// 位置の反映
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_world, &m_world, &translate);

	// ワールドビュープロジェクションマトリックス
	wvp = m_world * vp;

	//----------------------------
	// バーテックス
	//----------------------------
	if(vsc != nullptr)
	{
		// ワールドマトリックスの設定
		vsc->SetMatrix(m_device, "gWorld", &m_world);

		// ワールドビュープロジェクションマトリックスの設定
		vsc->SetMatrix(m_device, "gWVP", &wvp);
	}

	//----------------------------
	// サンプラー準備
	//----------------------------

	int texSumpler = -1;
	int norSumpler = -1;
	if(psc != nullptr)
	{
		// サンプラーインデックス
		texSumpler = psc->GetSamplerIndex("texSampler");
		
		// サンプラーステートパラメータ
		m_device->SetSamplerState(texSumpler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(U値)を設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(V値)を設定
		
		m_device->SetSamplerState(texSumpler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);	// テクスチャ縮小フィルタモードを設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大フィルタモードを設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAXANISOTROPY, 16);
		
		// サンプラーインデックス
		norSumpler = psc->GetSamplerIndex("norSampler");
		
		// サンプラーステートパラメータ
		m_device->SetSamplerState(norSumpler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(U値)を設定
		m_device->SetSamplerState(norSumpler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// テクスチャアドレッシング方法(V値)を設定
		
		m_device->SetSamplerState(norSumpler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);	// テクスチャ縮小フィルタモードを設定
		m_device->SetSamplerState(norSumpler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(norSumpler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大フィルタモードを設定
		m_device->SetSamplerState(norSumpler, D3DSAMP_MAXANISOTROPY, 16);
		
		
	}
	
	//----------------------------
	// テクスチャ設定
	//----------------------------
	// テクスチャの設定
	if(m_texture != nullptr)
	{
		if(texSumpler >= 0)
			m_device->SetTexture(texSumpler, m_texture);
		else
			m_device->SetTexture(0, m_texture);
	}
	
	// 法線テクスチャの設定
	if(m_norTexture != nullptr)
		if(norSumpler >= 0)
			m_device->SetTexture(norSumpler, m_norTexture);
			
			
	//----------------------------
	// 描画
	//----------------------------
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vtx, sizeof(VERTEX));
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void Polygon3D::SetVertex(void)
{
	// 座標
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f * m_scl.x, 0.5f * m_scl.y, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x,  half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x, -half.y, half.z);

	// テクスチャ座標
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点カラー
	m_vtx[0].color = m_color;
	m_vtx[1].color = m_color;
	m_vtx[2].color = m_color;
	m_vtx[3].color = m_color;

	// 法線ベクトル
	for(int cnt = 0; cnt < 4; ++cnt)
	{
		m_vtx[cnt].normal =  D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXVec3Normalize(&m_vtx[cnt].normal, &m_vtx[cnt].normal);
	}
}

//=============================================================================
// 頂点サイズ変換
//=============================================================================
void Polygon3D::SetVertexScl(void)
{
	// 座標
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f * m_scl.x, 0.5f * m_scl.y, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x,  half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x, -half.y, half.z);
}

//=============================================================================
// 頂点色変換
//=============================================================================
void Polygon3D::SetVertexColor(void)
{
	// 頂点カラー
	m_vtx[0].color = m_color;
	m_vtx[1].color = m_color;
	m_vtx[2].color = m_color;
	m_vtx[3].color = m_color;
}

// EOF
