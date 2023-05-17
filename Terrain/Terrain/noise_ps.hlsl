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
	//float random = frac(sin(value + mutator) * 143758.5453);
	//return k;
	uint n = asuint(value);
	n ^= (n << 13);
	n ^= (n >> 17);
	//n *= k;
	n ^= (n << 15);
	return float(n) / (float)UINT_MAX;
	//return random;
}


// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 pos    : SV_POSITION;
	float4 dif     : COLOR0;
	float2 uv  	: TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Output      : SV_TARGET0;
};

// C++ ���Őݒ肷��萔�̒�`

// �`�悷��e�N�X�`��
SamplerState smp       : register(s0);

SamplerState smp2
{
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D    tex       : register(t0);
Texture2D    norm       : register(t1);
cbuffer cb : register(b3) {
	float time;// : register(b3);
};

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	float2 offset= norm.Sample(smp2, PSInput.uv).rg;
	offset=(offset*2)-1;
	float2 uv = PSInput.uv +offset * 0.15;
	float t= Random(time);
	float t2 = Random(time+0.001);
	float rand= rand2dTo1d(uv+t);
	float rnd= rand2dTo1d(uv - fmod(uv, 0.02) + float2(t, time))*0.55;
	float4 texcol= tex.Sample(smp2, uv);
	PSOutput.Output = 1 - step(lerp(texcol.gbra, texcol, step(rnd, 0.5)), rand);
	return PSOutput;
}