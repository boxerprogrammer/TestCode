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
	float4 Base      : SV_TARGET0 ;//通常
	float4 Outline   : SV_TARGET1;//輪郭線
} ;

// C++ 側で設定する定数の定義

// 描画するテクスチャ
SamplerState smp       : register( s0 ) ;
Texture2D    tex       : register( t0 ) ;
Texture2D    norm       : register( t1 ) ;


float OutlineCheck(float2 uv){
	#define DIV_SQRT_2 0.70710678118
	float2 directions[8] = {
		float2(1, 0), float2(0, 1), float2(-1, 0), float2(0, -1),
	  float2(DIV_SQRT_2, DIV_SQRT_2), float2(-DIV_SQRT_2, DIV_SQRT_2),
	  float2(-DIV_SQRT_2, -DIV_SQRT_2), float2(DIV_SQRT_2, -DIV_SQRT_2)};
	float maxAlpha = 0.0f;
	//8近傍を見ながらその中でアルファ値が最も高い所を返す(levelは輪郭線の厚み)
	for(uint level=1;level<=1;++level){
		for(uint index = 0; index<8; index++){
			float2 sampleUV = uv + directions[index] * float(level)*0.01;
			maxAlpha = max(maxAlpha, tex.Sample(smp,sampleUV).a/float(level));
		}
		// float2 sampleUV = uv + float2(-DIV_SQRT_2, -DIV_SQRT_2)* float(level)*0.008;
		// maxAlpha = max(maxAlpha, tex.Sample(smp,sampleUV).a/float(level));
	}
	return maxAlpha;
}

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{

	PS_OUTPUT PSOutput ;
	
	float4 col=tex.Sample(smp,PSInput.uv);
	float maxAlpha=OutlineCheck(PSInput.uv);//8近傍チェックし最大アルファを取得

	//ここで得られたアルファ値は基本的に０か１である。
	//つまり1の部分に色を付ければそれが輪郭線となるわけである。
	PSOutput.Base.rgb =  lerp(float3(0.5, 0.5, 1), col.rgb, col.a);
	PSOutput.Base.a = max(col.a, maxAlpha);

	PSOutput.Outline.rgb =  lerp(float3(0.5, 0.5, 1), float3(0, 0, 0), col.a);
	PSOutput.Outline.a =  maxAlpha;
	
	return PSOutput ;
}
