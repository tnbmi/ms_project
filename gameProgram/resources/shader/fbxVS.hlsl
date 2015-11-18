float4x4 mtx_vp;
float4 mat_diffuse;

float4x4 mtx_bone[50];

float3 light_vec;

sampler samp;
void VS( in float3 Pos : POSITION,in float3 Nor : NORMAL,in float2 Uv : TEXCOORD0,in float4 Weight : TEXCOORD1,in float4 BoneIdx :TEXCOORD2,
         out float4 OutPos : POSITION,out float4 OutCol : COLOR,out float2 OutUv : TEXCOORD0 )
{
    int idx[4];
    idx[0] = BoneIdx[0];
    idx[1] = BoneIdx[1];
    idx[2] = BoneIdx[2];
    idx[3] = BoneIdx[3];
    
    float4x4 mtx_comb_world =0.0f;

    for( int i = 0 ; i < 4 ; i++ )
    {
        if( idx[i] == -1 )
        {
           break;
        }

        mtx_comb_world += mtx_bone[ idx[i] ] * Weight[i];
    }


    float4 outp;
    
    outp = mul( float4(Pos,1),mtx_comb_world );
    outp = mul( outp,mtx_vp );


    OutUv = Uv;
    OutPos = outp;
    float3 n = mul( Nor,mtx_comb_world );
    OutCol = mat_diffuse * (dot( n,-light_vec ) * 0.5 + 0.5);
}