Texture2D tex : register(t0);//通常のテクスチャ
Texture2D<float> depthTex : register(t1);//深度値テクスチャ
Texture2D<float3> normalTex : register(t2);

SamplerState smp : register(s0);

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
    float2 pixDif = float2(1.0 / (float) w, 1.0 / (float) h);
    float outline = depthTex.Sample(smp, input.uv) * 4 - //中心
                     depthTex.Sample(smp, input.uv - float2(0, pixDif.y)) - //上
                     depthTex.Sample(smp, input.uv + float2(0, pixDif.y)) - //下
                    depthTex.Sample(smp, input.uv - float2(pixDif.x,0)) - //左
                    depthTex.Sample(smp, input.uv + float2(pixDif.x,0));
    
    float3 outlineNormal = normalTex.Sample(smp, input.uv) * 4 - //中心
                     normalTex.Sample(smp, input.uv - float2(0, pixDif.y)) - //上
                     normalTex.Sample(smp, input.uv + float2(0, pixDif.y)) - //下
                    normalTex.Sample(smp, input.uv - float2(pixDif.x, 0)) - //左
                    normalTex.Sample(smp, input.uv + float2(pixDif.x, 0));
    
    outline = step(saturate(outline), 0.01);
    float outlineN = step(length(saturate(outlineNormal)),0.25);
    outline *= outlineN;
    float4 col = tex.Sample(smp, input.uv);
    return float4(col.rgb, col.a);
}