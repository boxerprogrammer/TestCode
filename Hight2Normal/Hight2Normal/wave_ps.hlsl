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

cbuffer Constant : register(b3) {
	float angle;
}

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{

	PS_OUTPUT PSOutput ;
	
	const float level = 20;
	const float eps=1.0f/360.0;
	float2 xy = PSInput.uv - 0.5;
	float len=1-clamp(length(PSInput.uv-0.5),0.0,0.5);
	float pangle = atan2(xy.y, xy.x);
	float h = pow(2 * (0.5 - len)*(1.25+sin(pangle+3.14*0.5)), 2.0f) * ((cos(abs(len - angle) * 20.0) + 1) / 2.0);// (sin(abs(len - angle) / 10) + 1) / 2;// pow(pow(0.9, 1.0f - (len - angle)), angle);// pow(1.0f - (abs(len - angle)), 2);// 10.0 / len * (sin(len * level - angle) + 1) / 2;

	PSOutput.Output.rgb = 1-h;
	PSOutput.Output.a=1;
	
	return PSOutput ;
}
