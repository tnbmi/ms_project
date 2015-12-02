//*****************************************************************************
//
// �p�[�s�N�Z���V�F�[�_�[ [perPixelVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �O���󂯎��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4x4 gWVP;
float4x4 gWorld;

//=============================================================================
// �V�F�[�_�[�֐�
//=============================================================================
void VS(in float3 inPos			: POSITION0,
		in float3 inNormal		: NORMAL0,
		in float2 inUV			: TEXCOORD0,
		out float4 outPos		: POSITION,
		out float2 outUV		: TEXCOORD0,
		out float4 outWorldPos	: TEXCOORD1,
		out float3 outWorldNor	: TEXCOORD2)
{
	// �o�͍��W�ϊ�
	outPos = mul(float4(inPos, 1.0f), gWVP);

	// �e�N�X�`��UV
	outUV = inUV;

	// �o�̓��[���h���W�ϊ�
	outWorldPos = mul(float4(inPos, 1.0f), gWorld);

	// �o�͕ϊ��@���i���[���h�j
	outWorldNor = mul(float4(inNormal, 0.0f), gWorld).xyz;
}
