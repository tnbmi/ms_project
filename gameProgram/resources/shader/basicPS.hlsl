//*****************************************************************************
//
// ベーシックシェーダー [basicPS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 外部受け取り
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sampler texSampler;
sampler subSampler;

//=============================================================================
// シェーダー関数
//=============================================================================
float4 PS(float4 diffuse : COLOR0,
		  float2 UV		 : TEXCOORD0,
		  float2 subUV	 : TEXCOORD1) : COLOR0
{
	//return float4(tex2D(texSampler, UV).rgb * tex2D(subSampler, subUV).rgb * diffuse.rgb, 1.0f);
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
