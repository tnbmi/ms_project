//*****************************************************************************
//
// ���C�e�B���O�V�F�[�_�[ [lightVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �O���󂯎��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4x4 gWVP;
float4x4 gWorld;

float3 gDirLightVector;
float4 gDirLightDiffuse;
float4 gDirLightAmbient;

float4 gMatDiffuse;
float4 gMatAmbient;

//=============================================================================
// �V�F�[�_�[�֐�
//=============================================================================
void VS(in float3 inPos			: POSITION0,
		in float3 inNormal		: NORMAL0,
		in float2 inUV			: TEXCOORD0,
		out float4 outPos		: POSITION,
		out float4 outDiffuse	: COLOR0,
		out float2 outUV		: TEXCOORD0)
{
	// �o�͍��W�ϊ�
	outPos = mul(float4(inPos, 1.0f), gWVP);

	// �o�̓��[���h�@���ϊ�
	float3 worldNormal = mul(float4(inNormal, 0.0f), gWorld);
	worldNormal = normalize(worldNormal);

	// �e�N�X�`��UV
	outUV = inUV;

	// ���C�g�v�Z
	float light = (dot(worldNormal, -gDirLightVector) * 0.5f) + 0.5f;

	// �o�̓J���[
	outDiffuse = light * gMatDiffuse * gDirLightDiffuse + gMatAmbient * gDirLightAmbient;
	outDiffuse.a = 1.0f;
}
