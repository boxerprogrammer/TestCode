// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 pos    : SV_POSITION ;
	float4 dif     : COLOR0 ;
	float2 uv  : TEXCOORD0 ;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Output      : SV_TARGET0 ;
} ;

// C++ 側で設定する定数の定義

// 描画するテクスチャ
SamplerState smp       : register( s0 ) ;
Texture2D    tex       : register( t0 ) ;
Texture2D    norm       : register( t1 ) ;

#define DIV_SQRT_2 0.70710678118
float2 directions[8] = {float2(1, 0), float2(0, 1), float2(-1, 0), float2(0, -1),
  float2(DIV_SQRT_2, DIV_SQRT_2), float2(-DIV_SQRT_2, DIV_SQRT_2),
  float2(-DIV_SQRT_2, -DIV_SQRT_2), float2(DIV_SQRT_2, -DIV_SQRT_2)};
// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	
	float4 col=tex.Sample(smp,PSInput.uv);
	float maxAlpha = 0;
	for(uint index = 0; index<8; index++){
		float2 sampleUV = PSInput.uv + directions[index] * 0.01;
		maxAlpha = max(maxAlpha, tex.Sample(smp,sampleUV).a);
	}
	PSOutput.Output.rgb = lerp(float3(0.5, 0.5, 1), col.rgb, col.a);
	PSOutput.Output.a=max(col.a,maxAlpha);
	return PSOutput ;
}
