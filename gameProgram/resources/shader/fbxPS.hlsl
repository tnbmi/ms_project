float4x4 mtx_vp;
float4 mat_diffuse;

float4x4 mtx_bone[50];

sampler samp;

void PS( in float2 Uv : TEXCOORD0,in float4 Col : COLOR,out float4 OutCol : COLOR )
{
    OutCol =  tex2D(samp,Uv) * Col;
    OutCol.a = 1.0;
}