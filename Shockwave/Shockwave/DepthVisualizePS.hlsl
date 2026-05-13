
Texture2D depth : register(t0);
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
    float d = depth.Sample(smp, input.uv).r;
    d = pow(1 - d, 1);
    return float4(d, d, d, 1);
}