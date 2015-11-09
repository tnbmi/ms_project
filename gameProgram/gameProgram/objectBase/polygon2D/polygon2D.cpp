//*****************************************************************************
//
// 2Dポリゴンの基盤 [polygon2D.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "polygon2D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
Polygon2D::Polygon2D(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type) : ObjectBase(device, objectList, priority, type)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_scl = D3DXVECTOR3(128.0f, 128.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
Polygon2D::~Polygon2D(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Polygon2D::Create(Polygon2D** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, LPDIRECT3DTEXTURE9 texture, int priority, OBJECT_TYPE type)
{
	Polygon2D* pointer = new Polygon2D(device, objectList, priority, type);
	if(!pointer->Initialize(texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Polygon2D::Initialize(LPDIRECT3DTEXTURE9 texture)
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
void Polygon2D::Finalize(void)
{
	ObjectBase::~ObjectBase();
}

//=============================================================================
// 更新
//=============================================================================
void Polygon2D::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void Polygon2D::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
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

	//----------------------------
	// ワールドデータ設定
	//----------------------------
	// バーテックス
	if(vsc != nullptr)
	{
		// ワールドマトリックスの設定
		vsc->SetMatrix(m_device, "gWorld", &m_world);
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
	{
		//テクスチャの設定
		m_device->SetTexture(texSumpler, m_texture);
	}
	else
	{
		m_device->SetTexture(0, m_texture);
	}

	//----------------------------
	// 描画
	//----------------------------
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
								2,
								m_vtx,
								sizeof(VERTEX));
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void Polygon2D::SetVertex(void)
{
	// 座標
	D3DXVECTOR3 half = D3DXVECTOR3(m_scl.x * 0.5f, m_scl.y * 0.5f, 0.0f);
	m_vtx[0].position = D3DXVECTOR3(-half.x, -half.y, half.z);
	m_vtx[1].position = D3DXVECTOR3( half.x, -half.y, half.z);
	m_vtx[2].position = D3DXVECTOR3(-half.x,  half.y, half.z);
	m_vtx[3].position = D3DXVECTOR3( half.x,  half.y, half.z);

	// 反射光
	m_vtx[0].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	m_vtx[1].color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	m_vtx[2].color = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	m_vtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	m_vtx[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	m_vtx[1].uv = D3DXVECTOR2(1.0f, 0.0f);
	m_vtx[2].uv = D3DXVECTOR2(0.0f, 1.0f);
	m_vtx[3].uv = D3DXVECTOR2(1.0f, 1.0f);
}

// EOF
