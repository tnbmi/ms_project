//*****************************************************************************
//
// �@���}�b�v�V�F�[�_�[(���s����) [normalMapDirVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �O���󂯎��
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
// �V�F�[�_�[�֐�
//=============================================================================
float4 PS(float4 texUV		 : TEXCOORD0,
		  float4 worldPos	 : TEXCOORD1,
		  float3 worldNormal : TEXCOORD2) : COLOR0
{
	// �ϐ��錾
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

	// �@���}�b�v
	normalMap	= tex2D(norSampler, texUV).rgb;
	normalMap	= normalMap.rbg * 2 - 1;
	normalMap.y =  -normalMap.y;
	normalMap.z =  -normalMap.z;
	normalMap	= normalize(normalMap);

	// �@���ϊ��i���[���h�j
	worldNormal = mul(float4(normalMap, 0.0f), gWorld).xyz;
	worldNormal = normalize(worldNormal);

	// �e�N�X�`���J���[
	texColor = tex2D(texSampler, texUV).rgb;

	int cnt;
	for(cnt = 0; cnt < gNum; cnt++)
	{
		// ���C�g�v�Z
		dir = normalize(worldPos.xyz - gLightPos[cnt]);
		light = max(dot(worldNormal, -dir), 0);

		// ���C�e�B���O�v�Z
		lighting = light * gLightDiffuse[cnt].rgb * gMatDiffuse.rgb;

		// �X�y�L�����v�Z
		toEye = normalize(gCameraPos - worldPos.xyz);	// �J��������_�̃x�N�g��
		r = reflect(dir, normalize(worldNormal));		// ����
		s = pow(max(dot(r, toEye), 0.0f), 3.0f);		// �X�y�L�����p���[

		// �X�y�L�����v�Z
		spec = s * gLightDiffuse[cnt].rgb * float3(1.0f, 1.0f, 1.0f);

		// ���̌���
		d = distance(gLightPos[cnt], worldPos.xyz);	// ����
		a = gAttenuation[0] + gAttenuation[1] * d + gAttenuation[2] * d * d;

		// �J���[���Z
		color += (lighting * texColor + spec) / a;
	}

	return float4(color, 1.0f);
}
