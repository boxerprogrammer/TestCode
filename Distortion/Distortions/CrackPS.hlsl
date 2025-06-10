SamplerState smp : register(s0);
Texture2D tex : register(t0);
Texture2D normTex : register(t1);

struct PsInput
{
    float4 pos : SV_Position;
    float4 col : COLOR0;
    //float4 spc : COLOR1;
    float2 uv : TEXCOORD;
};

float4 main(PsInput input) : SV_TARGET
{
    uint2 gsize;
    uint levels;
    tex.GetDimensions(0, gsize.x, gsize.y, levels);
    float2 pixDiff = 50.0 / (float2)gsize;
    //画面ずらし用の画像から、rg成分だけを持ってきます
    float2 shiftUV = normTex.Sample(smp, input.uv).rg;
    shiftUV = shiftUV * 2.0 - 1.0;
    //このshiftUVをもとに、元画像の参照場所をずらします
    float4 col = tex.Sample(smp, input.uv+shiftUV*pixDiff);
    return col;
}