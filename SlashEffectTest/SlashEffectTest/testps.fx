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


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	
//	float2 fmd=fmod(PSInput.TexCoords0,0.1);
//	if(((fmd.x<=0.05) + (fmd.y<=0.05))%2==0 ){
//		PSOutput.Output=float4(1,1,0,1);
//	}else{
//		PSOutput.Output=tex.Sample( smp, PSInput.TexCoords0 ) ;
//	}
	float4 nuv=norm.Sample(smp,PSInput.uv);
	PSOutput.Output=PSInput.dif*PSInput.dif+tex.Sample( smp, fmod(PSInput.uv+(nuv.xy*2-float2(1,1))/32.0f,float2(1.0,1.0))  );
	// 関数の戻り値がラスタライザに渡される
	return PSOutput ;
}
