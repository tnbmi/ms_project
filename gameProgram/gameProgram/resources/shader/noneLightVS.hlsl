//*****************************************************************************
//
// ライティングシェーダー [lightVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 外部受け取り
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4x4 gWVP;
float4x4 gWorld;

//=============================================================================
// シェーダー関数
//=============================================================================
void VS(in float3 inPos			: POSITION0,
		in float3 inNormal		: NORMAL0,
		in float4 inDiffuse		: COLOR0,
		in float2 inUV			: TEXCOORD0,
		out float4 outPos		: POSITION,
		out float4 outDiffuse	: COLOR0,
		out float2 outUV		: TEXCOORD0)
{
	// 出力座標変換
	outPos = mul(float4(inPos, 1.0f), gWVP);

	// 出力ワールド法線変換
	float3 worldNormal = mul(float4(inNormal, 0.0f), gWorld);
	worldNormal = normalize(worldNormal);

	// テクスチャUV
	outUV = inUV;

	// 出力カラー
	outDiffuse = inDiffuse;
	outDiffuse.a = 1.0f;
}
