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
	float4 Base      : SV_TARGET0 ;
	float4 Bubble      : SV_TARGET1;
} ;

// C++ 側で設定する定数の定義

// 描画するテクスチャ
SamplerState smp       : register( s0 ) ;
Texture2D    tex       : register( t0 ) ;
Texture2D    aura       : register( t1 ) ;
float angle : register( c0 ) ;


float OutlineCheck(float2 uv){
		#define DIV_SQRT_2 0.70710678118
float2 directions[8] = {float2(1, 0), float2(0, 1), float2(-1, 0), float2(0, -1),
  float2(DIV_SQRT_2, DIV_SQRT_2), float2(-DIV_SQRT_2, DIV_SQRT_2),
  float2(-DIV_SQRT_2, -DIV_SQRT_2), float2(DIV_SQRT_2, -DIV_SQRT_2)};
	float maxAlpha = 0.0f;
	float auraLevel=aura.Sample(smp,uv+float2(0,angle)).r;
	for(uint level=1;level<=16;++level){
		for(uint index = 0; index<8; index++){
			float2 sampleUV = uv + directions[index] * float(level)*0.01*auraLevel;
			maxAlpha = max(maxAlpha, tex.Sample(smp,sampleUV).a/pow(float(level),0.8));
		}
	}
	return maxAlpha;
}


//float SDFLatticeCircle2D(float2 xy, float divider,float r) {
//	return length(fmod(xy, divider) - divider / 2) - r;
//}
//
//
// main関数
//PS_OUTPUT main( PS_INPUT PSInput )
//{
//
//	PS_OUTPUT PSOutput ;
//	
//	float4 col=tex.Sample(smp,PSInput.uv);
//	float maxAlpha=OutlineCheck(PSInput.uv);
//
//
//	float a=SDFLatticeCircle2D(PSInput.uv,maxAlpha,maxAlpha*0.25);
//	if(a<0.0f){
//		a=1.0f;
//	}else{
//		a=0.0f;
//	}
//	PSOutput.Output.rgb = lerp(float3(0.5, 0.75, 1), col.rgb, col.a);
//	PSOutput.Output.a=max(col.a,a);
//	
//	return PSOutput ;
//}





// float SDFLatticeCircle2D(float2 xy, float divider,float r) {
// 	return length(fmod(xy, divider) - divider / 2) - r;
// }


// // main関数
// PS_OUTPUT main( PS_INPUT PSInput )
// {

// 	PS_OUTPUT PSOutput ;
	
// 	float4 col=tex.Sample(smp,PSInput.uv);
// 	float maxAlpha=OutlineCheck(PSInput.uv);


// 	float a=SDFLatticeCircle2D(PSInput.uv,maxAlpha*0.5,maxAlpha);
// 	if(a<0.0f){
// 		a=1.0f;
// 	}else{
// 		a=0.0f;
// 	}
	
	
	
//邪悪	
float sdCircle( float2 p, float r )
{
	return length(fmod(p,r)) - r*0.5;
}

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{

	PS_OUTPUT PSOutput ;
	
	float4 col=tex.Sample(smp,PSInput.uv);
	float maxAlpha=OutlineCheck(PSInput.uv);


	float a=sdCircle(PSInput.uv,maxAlpha);
	PSOutput.Base.rgb = lerp(float3(1, 0.75,0.5), col.rgb, col.a);
	PSOutput.Base.a=max(col.a,a*10);
	PSOutput.Bubble.rgb= lerp(float3(1, 0.75, 0.5), float3(0,0,0), col.a);
	PSOutput.Bubble.a = a*10;
	return PSOutput ;
}


/*しっとり
// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{

	PS_OUTPUT PSOutput ;
	
	float4 col=tex.Sample(smp,PSInput.uv);
	float maxAlpha=OutlineCheck(PSInput.uv);

	

	PSOutput.Output.rgb = lerp(float3(0.5, 0.75, 1), col.rgb, col.a);
	PSOutput.Output.a=max(col.a,maxAlpha*2);
	
	return PSOutput ;
}
*/