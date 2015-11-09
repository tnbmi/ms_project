//*****************************************************************************
//
// 二次元用シェーダー [twoDimensionsVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 外部受け取り
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4x4 gProj;
float4x4 gWorld;

//=============================================================================
// シェーダー関数
//=============================================================================
void VS(in	float3 inPosition	: POSITION0,
		//in	float4 inColor		: COLOR0,
		//in	float2 inTexcoord	: TEXCOORD0,
		out	float4 outPosition	: POSITION/*,
		out	float4 outColor		: COLOR0,
		out	float2 outTexcoord	: TEXCOORD0*/)
{
	//----------------------------
	// 座標の二次元変換
	//----------------------------
	outPosition.x = inPosition.x / 640.0f;
	outPosition.y = -(inPosition.y / 360.0f);
	outPosition.z = 0.0f;
	outPosition.w = 1.0f;

	// ワールド変換
	//float4 worldPosition;
	//worldPosition = mul(float4(inPosition, 1.0f), gWorld);

	// プロジェクション
	//outPosition = mul(inPosition, gProj);

	//outPosition = float4(inPosition, 1.0f);

	//----------------------------
	// アウトへ渡す
	//----------------------------
	// テクスチャ座標
	//outTexcoord = inTexcoord;

	// 色
	//outColor = inColor;
}

// EOF
