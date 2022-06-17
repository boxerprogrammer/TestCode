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

cbuffer Consrtant : register(b3) {
	float angle;
}

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{

	PS_OUTPUT PSOutput ;
	
	const float level = 20;
	const float eps=1.0f/360.0;

	float len=length(PSInput.uv-0.5);
	
	PSOutput.Output.rgb =10.0/len*(sin(len*level - angle)+1)/2;
	PSOutput.Output.a=1;
	
	return PSOutput ;
}
