//*****************************************************************************
//
// パーピクセルシェーダー [perPixelVS.hlsl]
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
		in float2 inUV			: TEXCOORD0,
		out float4 outPos		: POSITION,
		out float2 outUV		: TEXCOORD0,
		out float4 outWorldPos	: TEXCOORD1,
		out float3 outWorldNor	: TEXCOORD2)
{
	// 出力座標変換
	outPos = mul(float4(inPos, 1.0f), gWVP);

	// テクスチャUV
	outUV = inUV;

	// 出力ワールド座標変換
	outWorldPos = mul(float4(inPos, 1.0f), gWorld);

	// 出力変換法線（ワールド）
	outWorldNor = mul(float4(inNormal, 0.0f), gWorld).xyz;
}
