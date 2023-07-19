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

// C++ 側で設定する定数の定義
float time : register(c0);

// 描画するテクスチャ
SamplerState smp       : register(s0);
Texture2D<float4>    tex       : register(t0);
Texture2D<float4>    cloudTex       : register(t1);
Texture2D<float4>    dotTex       : register(t2);


float OutlineCheck(float2 uv) {
#define DIV_SQRT_2 0.70710678118
	float2 directions[8] = { float2(1, 0), float2(0, 1), float2(-1, 0), float2(0, -1),
		float2(DIV_SQRT_2, DIV_SQRT_2), float2(-DIV_SQRT_2, DIV_SQRT_2),
		float2(-DIV_SQRT_2, -DIV_SQRT_2), float2(DIV_SQRT_2, -DIV_SQRT_2) };
	float maxAlpha = 0.0f;
	for (uint level = 1; level <= 1; ++level) {
		for (uint index = 0; index < 8; index++) {
			float2 sampleUV = uv + directions[index] * float(level) * 0.01;
			maxAlpha = max(maxAlpha, tex.Sample(smp, sampleUV).a / float(level));
		}
		// float2 sampleUV = uv + float2(-DIV_SQRT_2, -DIV_SQRT_2)* float(level)*0.008;
		// maxAlpha = max(maxAlpha, tex.Sample(smp,sampleUV).a/float(level));
	}
	return maxAlpha;
}

// main関数
float4 main(PS_INPUT PSInput) :SV_TARGET
{
	#define DIV_SQRT_2 0.70710678118
	float2 directions[8] = { float2(1, 0), float2(0, 1), float2(-1, 0), float2(0, -1),
		float2(DIV_SQRT_2, DIV_SQRT_2), float2(-DIV_SQRT_2, DIV_SQRT_2),
		float2(-DIV_SQRT_2, -DIV_SQRT_2), float2(DIV_SQRT_2, -DIV_SQRT_2) };

	float w,h,levels;
	dotTex.GetDimensions(0, w, h,levels);
	float dx = 1.0f / w;
	PS_OUTPUT PSOutput;
	float4 col = tex.Sample(smp, PSInput.uv);
	float2 uvo = PSInput.uv;
	float t = time * 0.1;
	float d = dotTex.Sample(smp, fmod(uvo + float2(t, 0), 1.0)).r;
	float2 uv = uvo;
	

	if (d ==0.0) {
		for (uint level = 1; level <= 8; ++level) {
			for (uint index = 0; index < 8; index++) {
				float2 sampleUV = uvo + directions[index] * float(level) * 0.001;
				d = max(d, dotTex.Sample(smp, fmod(sampleUV + float2(t*4, 0), 1.0)).a / float(level));
				if (d > 0.0) {
					uv = sampleUV;
				}
			}
		}
	}

	if (col.a == 0.0 && d>0.25) {
		float2 offset = float2(0.0,0.0);
		for (uint level = 0; level < 32; ++level) {
			offset.x += dx;
			col=tex.Sample(smp, uv+offset);
			if (col.a == 1.0) {
				col.a =  (1.0 / (level*0.15 + 0.75));
				return col;
			}
		}
	}
	return col;

	//float maxAlpha = OutlineCheck(PSInput.uv);
	//PSOutput.Output.rgb = lerp(float3(0.5, 0.5, 1), col.rgb, col.a);
	//PSOutput.Output.a = max(col.a, maxAlpha);

	//return PSOutput;
}
