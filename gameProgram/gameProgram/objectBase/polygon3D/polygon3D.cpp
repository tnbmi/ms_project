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
	m_rot.x += 1.0f;
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
bool Polygon3D::Create(Polygon3D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, OBJECT_TYPE type)
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
	// スケールの反映
	D3DXMatrixScaling(&scaling, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_world, &m_world, &scaling);
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

		// マテリアル設定
		D3DXVECTOR3 ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		vsc->SetVector(m_device, "gMatDiffuse", (D3DXVECTOR4*)&m_color);
		vsc->SetVector(m_device, "gMatAmbient", (D3DXVECTOR4*)&ambient);
	}

	//----------------------------
	// サンプラー準備
	//----------------------------
	unsigned int texSumpler = -1;
	if(psc != nullptr)
	{
		// サンプラーインデックス
		texSumpler = psc->GetSamplerIndex("texSampler");

		// サンプラーステートパラメータ
		m_device->SetSamplerState(texSumpler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);	// テクスチャ縮小フィルタモードを設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大フィルタモードを設定
		m_device->SetSamplerState(texSumpler, D3DSAMP_MAXANISOTROPY, 16);
	}

	//----------------------------
	// テクスチャ設定
	//----------------------------
	if(texSumpler >= 0)
		//テクスチャの設定
		m_device->SetTexture(texSumpler, m_texture);
	else
		m_device->SetTexture(0, m_texture);

	//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//----------------------------
	// 描画
	//----------------------------
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
								2,
								m_vtx,
								sizeof(VERTEX));

	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void Polygon3D::SetVertex(void)
{
	// 座標
	D3DXVECTOR3 half = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x,  half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x, -half.y, half.z);

	// テクスチャ座標
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	// 法線ベクトル
	for(int cnt = 0; cnt < 4; ++cnt)
	{
		m_vtx[cnt].normal =  D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		D3DXVec3Normalize(&m_vtx[cnt].normal, &m_vtx[cnt].normal);
	}
}

// EOF
