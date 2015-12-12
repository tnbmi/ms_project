sampler samp;

void PS( in float2 Uv : TEXCOORD0 ,in float4 Col : COLOR,out float4 OutCol : COLOR )
{
	OutCol = tex2D( samp,Uv ) * Col;
} 