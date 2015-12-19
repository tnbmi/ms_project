//*****************************************************************************
//
// メッシュのドーム [meshDome.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshDome.h"
#include "..\..\common\safe.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義計算
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define VERTEX_NUM(x, z)		((x + 1) * (z + 1))					// 総頂点計算
#define INDEX_NUM(x, z)		((x + 1) * (z * 2) + ((z - 1) * 2))	// 総インデックス計算
#define WORK_LINE(x, z, idx)	((idx / (INDEX_NUM(x, z) / z)) + 1)	// 一行のインデックス計算

//=============================================================================
// コンストラクタ
//=============================================================================
MeshDome::MeshDome(LPDIRECT3DDEVICE9 device, ObjectList* objectList, OBJECT_TYPE type) : ObjectBase(device, objectList, type)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_vtx = nullptr;
	m_idx = nullptr;

	m_blockNum	= D3DXVECTOR2(0.0f, 0.0f);

	m_vtxNum	 = 0;
	m_indexNum	 = 0;
	m_polygonNum = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
MeshDome::~MeshDome(void)
{
	SafeDelete(m_vtx);
	SafeDelete(m_idx);
}

//=============================================================================
// 生成
//=============================================================================
bool MeshDome::Create(MeshDome** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList,
					D3DXVECTOR2 blockNum, float radius, float height,
					LPDIRECT3DTEXTURE9 texture,OBJECT_TYPE type)
{
	MeshDome* pointer = new MeshDome(device, objectList, type);
	if(!pointer->Initialize(blockNum, radius, height, texture))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool MeshDome::Initialize(D3DXVECTOR2 blockNum, float radius, float height, LPDIRECT3DTEXTURE9 texture)
{
	//----------------------------
	// ドームに出来るかのチェック
	//----------------------------
	if(blockNum.x < 3)
		return false;

	//----------------------------
	// ブロック設定
	//----------------------------
	// ブロック数
	m_blockNum.x = blockNum.x;
	m_blockNum.y = blockNum.y;

	// 半径
	m_scl.x = radius;
	m_scl.z = radius;

	// 高さ
	m_scl.y = height;

	//----------------------------
	// ドーム設定
	//----------------------------
	// 総頂点数
	m_vtxNum = (int)VERTEX_NUM(m_blockNum.x, m_blockNum.y);

	// 頂点の総インデックス数
	m_indexNum = (int)INDEX_NUM(m_blockNum.x, m_blockNum.y);

	// 総ポリゴン数
	m_polygonNum = m_indexNum - 2;

	//----------------------------
	// 頂点設定
	//----------------------------
	// 頂点バッファ生成
	m_vtx = new VERTEX[m_vtxNum];
	m_idx = new int[m_indexNum];

	// 頂点情報設定
	SetVertex();

	// 頂点インデックス設定
	SetVertexIndex();

	//----------------------------
	// テクスチャ設定
	//----------------------------
	m_texture = texture;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void MeshDome::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void MeshDome::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void MeshDome::Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
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

	//----------------------------
	// 描画
	//----------------------------
	m_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, m_vtxNum, m_polygonNum,
									m_idx, D3DFMT_INDEX32, m_vtx, sizeof(VERTEX));
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void MeshDome::SetVertex(void)
{
	int			workVtx;
	float		workAngleX = PAI * 2.0f / m_blockNum.x;
	float		workAngleY = PAI / (m_blockNum.y * 2.0f);
	D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / (float)m_blockNum.x,
									1.0f / (float)m_blockNum.y,
									0.0f);

	//----------------------------
	// 行の処理
	//----------------------------
	for(float cntY = 0; cntY <= m_blockNum.y; ++cntY)
	{
		//----------------------------
		// 列の処理
		//----------------------------
		for(float cntX = 0; cntX <= m_blockNum.x; ++cntX)
		{
			// 頂点番号
			workVtx = (int)((cntY * (m_blockNum.x + 1)) + cntX);

			// 頂点座標
			m_vtx[workVtx].position.x = cos(workAngleX * cntX) * cos(workAngleY * cntY) * m_scl.x;
			m_vtx[workVtx].position.z = sin(workAngleX * cntX) * cos(workAngleY * cntY) * m_scl.z;
			m_vtx[workVtx].position.y = sin(workAngleY * cntY) * m_scl.y;

			// 頂点ベクトル
			m_vtx[workVtx].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光
			m_vtx[workVtx].color = m_color;

			// テクスチャ座標
			m_vtx[workVtx].uv = D3DXVECTOR2(1.0f - texcord.x * (float)cntX, 1.0f - texcord.y * (float)cntY);
		}
	}
}

//=============================================================================
// 頂点インデックス設定
//=============================================================================
void MeshDome::SetVertexIndex(void)
{	//----------------------------
	// インデックス設定
	//----------------------------
	for(int cnt = 0, cnt2 = 0; cnt < m_indexNum; ++cnt, ++cnt2)
	{
		// 0じゃない かつ 重複している
		if( cnt != 0
		&& (((int)(cnt - VERTEX_NUM(m_blockNum.x, 1)) % (int)(VERTEX_NUM(m_blockNum.x, 1) + 2)) == 0
		 || ((int)(cnt - (VERTEX_NUM(m_blockNum.x, 1) + 2)) % (int)(VERTEX_NUM(m_blockNum.x, 1) + 2)) == 0))
		{
			// 重複の処理
			m_idx[cnt] = m_idx[cnt - 1];
			cnt2--;
		}
		else
		{
			// 重複しない処理
			m_idx[cnt] = (WORD)(((m_blockNum.x + 1) + (cnt2 / 2)) - ((cnt2 % 2) * (m_blockNum.x + 1)));
		}
	}
}

//=============================================================================
// 頂点サイズ変換
//=============================================================================
void MeshDome::SetVertexScl(void)
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
void MeshDome::SetVertexColor(void)
{
	// 頂点カラー
	m_vtx[0].color = m_color;
	m_vtx[1].color = m_color;
	m_vtx[2].color = m_color;
	m_vtx[3].color = m_color;
}

// EOF
