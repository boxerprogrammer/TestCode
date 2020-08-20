// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Position    : SV_POSITION ;
	float4 Diffuse     : COLOR0 ;
	float2 TexCoords0  : TEXCOORD0 ;
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
	
//	float2 fmd=fmod(PSInput.TexCoords0,0.1);
//	if(((fmd.x<=0.05) + (fmd.y<=0.05))%2==0 ){
//		PSOutput.Output=float4(1,1,0,1);
//	}else{
//		PSOutput.Output=tex.Sample( smp, PSInput.TexCoords0 ) ;
//	}
	PSOutput.Output=float4(0.1,0.1,0.1,1)+tex.Sample( smp, PSInput.TexCoords0 ) ;
	// 関数の戻り値がラスタライザに渡される
	return PSOutput ;
}
