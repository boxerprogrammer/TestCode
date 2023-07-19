// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 pos    : SV_POSITION;
	float4 dif     : COLOR0;
	float2 uv  : TEXCOORD0;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Output      : SV_TARGET0;
};

// 描画するテクスチャ
SamplerState smp       : register(s0);
Texture2D<float4>    tex       : register(t0);

static const uint UINT_MAX = 0xffffffffu;

uint XorShift(uint n) {
	n ^= (n << 13);
	n ^= (n >> 17);
	n ^= (n << 15);
	return n;
}

float Random(float p) {
	return float(XorShift(asuint(p))) / float(UINT_MAX);
}
float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233)) {
	float2 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = Random(random);
	return random;
}
//const uint k = 0x456789abu;

float rand1dTo1d(float value, float mutator = 0.546) {
	uint n = asuint(value);
	n ^= (n << 13);
	n ^= (n >> 17);
	n ^= (n << 15);
	return float(n) / (float)UINT_MAX;
}


// main関数
float4 main(PS_INPUT PSInput) :SV_TARGET
{
	//float b = rand2dTo1d(PSInput.uv);
	float4 col = tex.Sample(smp,PSInput.uv);
	return col;
	//return float4(b, b, b, 1);
}
