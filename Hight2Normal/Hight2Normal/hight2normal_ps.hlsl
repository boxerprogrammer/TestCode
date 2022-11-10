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
	
	const float level = 2.5;
	const float eps=1.0/256.0;
	float2 xeps= float2(eps, 0);
	float2 yeps = float2(0, eps);

	 float3 N=normalize(float3(
					tex.Sample(smp, PSInput.uv + xeps).r - tex.Sample(smp, PSInput.uv - xeps).r,
	 				tex.Sample(smp, PSInput.uv + yeps).r - tex.Sample(smp, PSInput.uv - yeps).r,
	 				eps*level));

	 N = normalize(N + float3(0, 0, sqrt(1 - N.x * N.x - N.y * N.y)));

	//float3 NX=normalize(float3(eps,0,(tex.Sample(smp, PSInput.uv + xeps).r - tex.Sample(smp, PSInput.uv -xeps).r))/2);
	//float3 NY=normalize(float3(0,eps,(tex.Sample(smp, PSInput.uv + yeps).r - tex.Sample(smp, PSInput.uv - yeps).r))/2);
	//				
	 //float2 dx=ddx(PSInput.uv);
	 //float2 dy=ddy(PSInput.uv);
	 //float3 N=tex.SampleGrad(smp,PSInput.uv,dx,dy).xyz;

	//float3 N = normalize(cross(NX, NY));

	
	
	PSOutput.Output.rgb =  (N + 1) / 2;
	PSOutput.Output.a=1;
	
	return PSOutput ;
}
