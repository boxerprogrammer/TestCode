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
	float4 Base      : SV_TARGET;//通常
	//float4 Outline   : SV_TARGET1;//輪郭線
};

// C++ 側で設定する定数の定義
cbuffer buff: register(b0) {
	float angle;//光の角度
	
}


// 描画するテクスチャ
SamplerState smp       : register(s0);
Texture2D    tex       : register(t0); 
Texture2D    norm       : register(t1);
 


float SDFSphere3D(float3 pos, float r) {
	return length(pos - float3(0, 0, 5)) - r;
}
float3 rotate(float3 p, float theta, float3 axis) {
	float3 a = normalize(axis);
	float s = sin(theta);
	float c = cos(theta);
	float r = 1.0 - c;
	float3x3 m = float3x3(
		a.x * a.x * r + c,
		a.y * a.x * r + a.z * s,
		a.z * a.x * r - a.y * s,
		a.x * a.y * r - a.z * s,
		a.y * a.y * r + c,
		a.z * a.y * r + a.x * s,
		a.x * a.z * r + a.y * s,
		a.y * a.z * r - a.x * s,
		a.z * a.z * r + c
		);
	return mul(m, p);
}
float SDFRoundBox(float3 p, float b, float r)
{
	float3 pp = abs(rotate(p,angle,float3(0.5,1,0.5))) - b;
	return length(max(pp, 0.0)) + min(max(pp.x, max(pp.y, pp.z)), 0.0) - r;
}
float3 mod(float3 pos, float divider) {
	return pos - divider * floor(pos / divider);
}

float SDFRepeat_SDFRoundBox(float3 p,float b, float r, float divider)
{
	float3 pp = mod(p, divider) - 0.5 * divider;
	return SDFRoundBox(pp, b, r);
}

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	const float epsilon = 0.001f;
	float3 light=float3(1, -1, 1);
	light = normalize(light);
	PS_OUTPUT PSOutput;

	PSOutput.Base = float4(0, 0, 1, 1);
	float3 eye = float3(0, 0, -22.5);//視点
	float3 tpos = float3(PSInput.uv*float2(2,-2)-float2(-1,1), 0);
	float3 ray = normalize(tpos - eye);//レイベクトル(このシェーダ内では一度計算したら固定)
	const int num = 256;
	const float div = 20.0;
	const float bar = 1;
	const float round = 0.25;
	for (int i = 0; i < num; ++i) {
		float sdf = SDFRepeat_SDFRoundBox(eye, bar, round,div);
		
		if (sdf <= epsilon) {
			float3 epsx = float3(epsilon, 0, 0);
			float3 epsy = float3(0,epsilon, 0);
			float3 epsz = float3(0,0,epsilon);
			float3 n = float3(
				SDFRepeat_SDFRoundBox(eye + epsx, bar, round, div) - SDFRepeat_SDFRoundBox(eye - epsx, bar,round, div),
				SDFRepeat_SDFRoundBox(eye + epsy, bar, round, div) - SDFRepeat_SDFRoundBox(eye - epsy, bar, round, div),
				SDFRepeat_SDFRoundBox(eye + epsz, bar, round, div) - SDFRepeat_SDFRoundBox(eye - epsz, bar, round, div));
			n = normalize(n);
			float spec = pow(saturate(dot(reflect(light, n), -ray)), 10);
			float b = saturate(dot(-light, n));
			b = max(b, 0.2);
			float3 dif = float3(b, b, 0.5 * b);
			dif += spec;
			dif = max(dif,0.1);
			PSOutput.Base = float4(dif,i*0.1);
			return PSOutput;
		}
		eye += ray * sdf;
	}
	return PSOutput;
}
