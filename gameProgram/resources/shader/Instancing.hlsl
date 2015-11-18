

float4x4 mtx_vp;
float2 uv_size;


sampler samp;

void VSBILL( in float3 Pos : POSITION,in float2 Uv : TEXCOORD0,in float4 Mtx1:TEXCOORD1,in float4 Mtx2:TEXCOORD2,
		in float4 Mtx3 : TEXCOORD3,in float4 Mtx4 :TEXCOORD4,in float2 UvOffset:TEXCOORD5,in float4 Col : COLOR,
		out float4 OutPos : POSITION,out float2 OutUv : TEXCOORD0,out float4 OutCol : COLOR )
{
	float4x4 MtxWorld;

	MtxWorld[0][0] = Mtx1[0];
	MtxWorld[0][1] = Mtx1[1];
	MtxWorld[0][2] = Mtx1[2];
	MtxWorld[0][3] = Mtx1[3];

	MtxWorld[1][0] = Mtx2[0];
	MtxWorld[1][1] = Mtx2[1];
	MtxWorld[1][2] = Mtx2[2];
	MtxWorld[1][3] = Mtx2[3];

	MtxWorld[2][0] = Mtx3[0];
	MtxWorld[2][1] = Mtx3[1];
	MtxWorld[2][2] = Mtx3[2];
	MtxWorld[2][3] = Mtx3[3];

	MtxWorld[3][0] = Mtx4[0];
	MtxWorld[3][1] = Mtx4[1];
	MtxWorld[3][2] = Mtx4[2];
	MtxWorld[3][3] = Mtx4[3];

	float4 pos4;
	float4x4 mtx;

	mtx = mul( MtxWorld,mtx_vp );
	pos4 = mul( float4(Pos,1),mtx );	

	OutPos = pos4;
	OutCol = Col;
	OutUv  = Uv + uv_size * UvOffset;

}

void PSBILL( in float2 Uv : TEXCOORD0 ,in float4 Col : COLOR,out float4 OutCol : COLOR )
{
	OutCol = tex2D( samp,Uv ) * Col;
} 