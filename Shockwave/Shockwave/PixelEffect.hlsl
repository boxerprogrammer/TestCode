Texture2D tex : register(t0); //通常のテクスチャ
Texture2D<float> depthTex : register(t1); //深度値テクスチャ
Texture2D<float> stelthTex : register(t2); //深度値テクスチャ(ステルス用)
Texture2D stelthPattern : register(t3); //ステルステクスチャ
Texture2D<float> distanceTex : register(t4); //ステルステクスチャ

SamplerState smp : register(s0);

cbuffer ClickedPos : register(b5)
{
    float4 clickedpos;
};

struct PSInput
{
    float4 pos : SV_Position;
    float4 dif : COLOR0;
    float4 spc : COLOR1;
    float2 uv : TEXCOORD0;
    float2 suv : TEXCOORD1;
};

float4 main(PSInput input) : SV_TARGET
{
    uint w, h;
    tex.GetDimensions(w, h);
    float2 pixel = float2(1.0 / (float) w, 1.0 / (float) h);
    float clickedDepth = depthTex.Sample(smp, clickedpos.xy * pixel);
    float dep = depthTex.Sample(smp, input.uv);
    float stelth = stelthTex.Sample(smp, input.uv);
    
    float2 vec = 0;
    if (dep > clickedDepth)
    {
        float r = clickedpos.z;
        vec = input.uv / pixel - clickedpos.xy;
        float dis = length(vec);
        vec = normalize(vec);
        vec *= pow(1.0 - saturate(distance(dis, r) / r), 2)*0.05;
    }
    float3 stelthColor = float3(1.0, 1.0, 1.0);
    float4 col = tex.Sample(smp, input.uv - vec);

    float distanceValue = distanceTex.Sample(smp, input.uv);
    
    
    if (clickedpos.w>0.0 && clickedpos.w - 0.2 < distanceValue && distanceValue < clickedpos.w)
    {
        col= float4(col.rrr, col.a);
        if (dep<stelth )
        {
            col = stelthPattern.Sample(smp, input.uv);
        }
    }
    
    float threshold = pow(1.0 - distance(clickedpos.w, distanceValue), 20.0f);
    if (clickedpos.w >= 0.5 || clickedpos.w==0.0)
    {
        return col;
    }
    return lerp(col, float4(0, 1, 1, 1), threshold);
    

}