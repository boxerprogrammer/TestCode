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
float angle : register( c0 ) ;

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	
	float4 n=norm.Sample(smp,PSInput.uv);
	n.xyz=n.xyz*2-1;
	n.y=-n.y;
	PSOutput.Output=tex.Sample( smp, PSInput.uv)*
	saturate(saturate(dot(normalize(float3(cos(angle),sin(angle),0)),n.xyz))+0.5);
	float s=pow(
	saturate(normalize(
		reflect(
				float3(-cos(angle),-sin(angle),0)
				,n.xyz
			)
	).z),
	10.0);
	PSOutput.Output+=float4(s,s,s,1);
	// 関数の戻り値がラスタライザに渡される
	return PSOutput ;
}
