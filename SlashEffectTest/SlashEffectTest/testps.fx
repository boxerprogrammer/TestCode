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
	
	float4 nuv=norm.Sample(smp,PSInput.uv);
	//ノーマルマップの色を取得
	//ノーマルマップ(rgb)のrgをuvのオフセットとして
	//扱うことで、ピクセル単位の細かいゆがみを表現できる
	//どういうことかというと、
	PSOutput.Output=PSInput.dif*PSInput.dif+
		tex.Sample( smp, fmod(PSInput.uv+(nuv.rg*2.0-1.0f)/32.0f,float2(1.0,1.0))  );
	// 関数の戻り値がラスタライザに渡される
	PSOutput.Output=(1.0-PSOutput.Output);
	return PSOutput ;
}
