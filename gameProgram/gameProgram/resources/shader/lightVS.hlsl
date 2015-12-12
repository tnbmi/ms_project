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

float3 gDirLightVector;
float4 gDirLightDiffuse;
float4 gDirLightAmbient;

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

	// ワールド法線変換
	float3 worldNormal = mul(float4(inNormal, 0.0f), gWorld);
	worldNormal = normalize(worldNormal);

	// ライト計算
	float light = (dot(worldNormal, -gDirLightVector) * 0.5f) + 0.5f;

	// 出力カラー
	outDiffuse = light * inDiffuse * gDirLightDiffuse + gDirLightAmbient;
	outDiffuse.a = 1.0f;

	// テクスチャUV
	outUV = inUV;
}
