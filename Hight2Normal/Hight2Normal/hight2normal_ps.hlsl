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

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{

	PS_OUTPUT PSOutput ;
	
	float eps=0.001;
	float2 xeps= float2(eps, 0);
	float2 yeps = float2(0, eps);

	float3 N=normalize(float3(tex.Sample(smp, PSInput.uv + xeps).r - tex.Sample(smp, PSInput.uv -xeps).r,
					tex.Sample(smp, PSInput.uv + yeps).r - tex.Sample(smp, PSInput.uv - yeps).r,
					0));
	N=normalize(N+float3(0,0,1));

	PSOutput.Output.rgb = (N+1)/2;
	PSOutput.Output.a=1;
	
	return PSOutput ;
}
