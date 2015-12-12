//*****************************************************************************
//
// 法線マップシェーダー(平行光源) [normalMapDirVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 外部受け取り
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float3	gDirLightVector;
float4	gDirLightDiffuse;
float4	gDirLightAmbient;

float3 gEyePos;

sampler texSampler;
sampler norSampler;

//=============================================================================
// シェーダー関数
//=============================================================================
float4 PS(float4	diffuse	 : COLOR0,
		  float4	UV		 : TEXCOORD0,
		  float4	worldPos : TEXCOORD1,
		  float4x4	world	 : TEXCOORD2) : COLOR0
{
	// 法線マップ
	float3 normalMap = tex2D(norSampler, UV).rgb;
	normalMap	= normalMap.rbg * 2 - 1;
	normalMap.y = -normalMap.y;
	normalMap.z = -normalMap.z;
	normalMap	= normalize(normalMap);

	// 法線変換（ワールド）
	float3 worldNormal = mul(float4(normalMap, 0.0f), world).xyz;
	worldNormal = normalize(worldNormal);

	// テクスチャカラー
	float3 texColor = tex2D(texSampler, UV).rgb;

	// ライト計算
	float light = max(dot(worldNormal, -gDirLightVector), 0);

	// ライティング計算
	float3 lighting = (/*light */ diffuse * gDirLightDiffuse /*+ gDirLightAmbient*/).rgb;

	// スペキュラ計算
	float3	toEye	= normalize(gEyePos - worldPos.xyz);				// カメラから点のベクトル
	float3	r		= reflect(gDirLightVector, normalize(worldNormal));	// 反射
	float	s		= pow(max(dot(r, toEye), 0.0f), 3.0f);				// スペキュラパワー

	// スペキュラ計算
	float3 spec = s * gDirLightDiffuse.rgb * float3(1.0f, 1.0f, 1.0f);

	// カラー加算
	float3 color = lighting * texColor + spec;

	return float4(texColor, 1.0f);
}
