//*****************************************************************************
//
// �@���}�b�v�V�F�[�_�[(���s����) [normalMapDirVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �O���󂯎��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float3	gDirLightVector;
float4	gDirLightDiffuse;
float4	gDirLightAmbient;

float3 gEyePos;

sampler texSampler;
sampler norSampler;

//=============================================================================
// �V�F�[�_�[�֐�
//=============================================================================
float4 PS(float4	diffuse	 : COLOR0,
		  float4	UV		 : TEXCOORD0,
		  float4	worldPos : TEXCOORD1,
		  float4x4	world	 : TEXCOORD2) : COLOR0
{
	// �@���}�b�v
	float3 normalMap = tex2D(norSampler, UV).rgb;
	normalMap	= normalMap.rbg * 2 - 1;
	normalMap.y = -normalMap.y;
	normalMap.z = -normalMap.z;
	normalMap	= normalize(normalMap);

	// �@���ϊ��i���[���h�j
	float3 worldNormal = mul(float4(normalMap, 0.0f), world).xyz;
	worldNormal = normalize(worldNormal);

	// �e�N�X�`���J���[
	float3 texColor = tex2D(texSampler, UV).rgb;

	// ���C�g�v�Z
	float3	lightVec = float3(0.0f, -1.0f, -2.0f);
	float3	dir = normalize(lightVec);
	float	light = max(dot(worldNormal, dir), 0);

	// ���C�e�B���O�v�Z
	float3 lighting = (light * diffuse.rgb /** gDirLightDiffuse.rgb + gDirLightAmbient.rgb*/);

	// �X�y�L�����v�Z
	float3	toEye	= normalize(gEyePos - worldPos.xyz);	// �J��������_�̃x�N�g��
	float3	r		= reflect(-dir, normalize(worldNormal));	// ����
	float	s		= pow(max(dot(r, toEye), 0.0f), 3.0f);	// �X�y�L�����p���[

	// �X�y�L�����v�Z
	float3 spec = s * gDirLightDiffuse.rgb * float3(1.0f, 1.0f, 1.0f);
/**/
	// �J���[���Z
	float3 color = lighting * texColor;// + spec;

	return float4(color, 1.0f);
}
