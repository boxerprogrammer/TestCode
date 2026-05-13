#include"ShaderHeader.hlsli"

Texture2D tex : register(t0);
SamplerState smp : register(s0);

float4 main(PsInput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);
	return col.gbra;
}