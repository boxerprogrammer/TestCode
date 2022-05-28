float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233)) {
	float2 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return random;
}
float rand1dTo1d(float value, float mutator = 0.546) {
	float random = frac(sin(value + mutator) * 143758.5453);
	return random;
}

// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 pos    : SV_POSITION;
	float4 dif     : COLOR0;
	float2 uv  	: TEXCOORD0;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Output      : SV_TARGET0;
};

// C++ 側で設定する定数の定義

// 描画するテクスチャ
SamplerState smp       : register(s0);
Texture2D    tex       : register(t0);
Texture2D    norm       : register(t1);
float time : register(b0);

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float2 offset= norm.Sample(smp, PSInput.uv).rg;
	offset=(offset*2)-1;
	float2 uv = PSInput.uv;// +offset * 0.15;
	float t= rand1dTo1d(time);
	float t2 = rand1dTo1d(time+0.001);
	float rand= rand2dTo1d(uv+t);
	float rnd= rand2dTo1d(uv - fmod(uv, 0.02) + float2(t, time))*0.55;
	float4 texcol= tex.Sample(smp, uv);
	PSOutput.Output = texcol;// 1 - step(lerp(texcol.gbra, texcol, step(rnd, 0.5)), rand);
	return PSOutput;
}