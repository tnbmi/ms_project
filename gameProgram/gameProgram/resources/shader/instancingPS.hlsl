sampler samp;

void PS( in float2 Uv : TEXCOORD0 ,in float4 Col : COLOR,out float4 OutCol : COLOR )
{
	float4 tex = tex2D(samp, Uv);
	if(tex.a <= 0.5f)
		tex.a = 0;

	OutCol = tex * Col;
} 