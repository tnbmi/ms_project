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
void VS(in float3 inPos			 : POSITION0,
		in float3 inNormal		 : NORMAL0,
		in float4 inDiffuse		 : COLOR0,
		in float2 inUV			 : TEXCOORD0,
		out float4	 outPos		 : POSITION,
		out float4	 outDiffuse	 : COLOR0,
		out float2	 outUV		 : TEXCOORD0,
		out float4	 outWorldPos : TEXCOORD1,
		out float4x4 outWorld	 : TEXCOORD2)
{
	// 出力座標変換
	outPos = mul(float4(inPos, 1.0f), gWVP);

	// 出力カラー
	outDiffuse = inDiffuse;

	// テクスチャUV
	outUV = inUV;

	// 出力ワールド座標変換
	outWorldPos = mul(float4(inPos, 1.0f), gWorld);

	// ワールド引き継ぎ
	outWorld = gWorld;
}
