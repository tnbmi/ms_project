//*****************************************************************************
//
// �񎟌��p�V�F�[�_�[ [twoDimensionsVS.hlsl]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �O���󂯎��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4x4 gProj;
float4x4 gWorld;

//=============================================================================
// �V�F�[�_�[�֐�
//=============================================================================
void VS(in	float3 inPosition	: POSITION0,
		in	float4 inColor		: COLOR0,
		in	float2 inTexcoord	: TEXCOORD0,
		out	float4 outPosition	: POSITION,
		out	float4 outColor		: COLOR0,
		out	float2 outTexcoord	: TEXCOORD0)
{
	//----------------------------
	// ���W�̓񎟌��ϊ�
	//----------------------------
	// ���[���h�ϊ�
	float4 worldPosition;
	worldPosition = mul(float4(inPosition, 1.0f), gWorld);

	outPosition.x =  (worldPosition.x / 640.0f - 1.0f);
	outPosition.y = -(worldPosition.y / 360.0f - 1.0f);
	outPosition.z = 0.0f;
	outPosition.w = 1.0f;

	// �v���W�F�N�V����
	//outPosition = mul(inPosition, gProj);

	//outPosition = float4(inPosition, 1.0f);

	//----------------------------
	// �A�E�g�֓n��
	//----------------------------
	// �e�N�X�`�����W
	outTexcoord = inTexcoord;

	// �F
	outColor = inColor;
}

// EOF
