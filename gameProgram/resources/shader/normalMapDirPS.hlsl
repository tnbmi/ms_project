//*****************************************************************************
//
// 法線マップシェーダー(平行光源) [normalMapDirVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 外部受け取り
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4x4 gWorld;

float3	gDirLightVector;
float4	gDirLightDiffuse;
float4	gDirLightAmbient;
float	gAttenuation[3];

float4 gMatDiffuse;
float4 gMatAmbient;

float3 gCameraPos;

sampler texSampler;
sampler norSampler;

//=============================================================================
// シェーダー関数
//=============================================================================
float4 PS(float4 texUV		 : TEXCOORD0,
		  float4 worldPos	 : TEXCOORD1,
		  float3 worldNormal : TEXCOORD2) : COLOR0
{
	// 変数宣言
	float3	dir;
	float	light;
	float3	toEye;
	float3	r;
	float	s;
	float	d;
	float	a;
	float3	lighting;
	float3	spec;
	float3	texColor;
	float3	normalMap;
	float3	color = float3(0.0f, 0.0f, 0.0f);

	// 法線マップ
	normalMap	= tex2D(norSampler, texUV).rgb;
	normalMap	= normalMap.rbg * 2 - 1;
	normalMap.y =  -normalMap.y;
	normalMap.z =  -normalMap.z;
	normalMap	= normalize(normalMap);

	// 法線変換（ワールド）
	worldNormal = mul(float4(normalMap, 0.0f), gWorld).xyz;
	worldNormal = normalize(worldNormal);

	// テクスチャカラー
	texColor = tex2D(texSampler, texUV).rgb;

	int cnt;
	for(cnt = 0; cnt < gNum; cnt++)
	{
		// ライト計算
		dir = normalize(worldPos.xyz - gLightPos[cnt]);
		light = max(dot(worldNormal, -dir), 0);

		// ライティング計算
		lighting = light * gLightDiffuse[cnt].rgb * gMatDiffuse.rgb;

		// スペキュラ計算
		toEye = normalize(gCameraPos - worldPos.xyz);	// カメラから点のベクトル
		r = reflect(dir, normalize(worldNormal));		// 反射
		s = pow(max(dot(r, toEye), 0.0f), 3.0f);		// スペキュラパワー

		// スペキュラ計算
		spec = s * gLightDiffuse[cnt].rgb * float3(1.0f, 1.0f, 1.0f);

		// 光の減衰
		d = distance(gLightPos[cnt], worldPos.xyz);	// 距離
		a = gAttenuation[0] + gAttenuation[1] * d + gAttenuation[2] * d * d;

		// カラー加算
		color += (lighting * texColor + spec) / a;
	}

	return float4(color, 1.0f);
}
