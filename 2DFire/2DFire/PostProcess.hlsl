// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 dif         : COLOR0;		// ディフューズカラー
	float4 spc        : COLOR1;		// スペキュラカラー
	float2 uv      : TEXCOORD0;	// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color           : SV_TARGET0;	// 色
};
SamplerState sam;//サンプラ(後述)
Texture2D tex:register(t0);//テクスチャ
Texture2D norm:register(t1);//テクスチャ
cbuffer ConstantBuffer : register(b0) {
	float threthold;
	float angle;
}
PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float2 n=norm.Sample(sam, input.uv).xy;
	n = n * 2 - 1;//-1～1
	
	float2 ygm = n.xy*0.1 ;
	ygm = float2(cos(angle) * ygm.x + sin(angle) * ygm.y,
		-sin(angle) * ygm.x + cos(angle) * ygm.y);
	output.color = tex.Sample(sam, input.uv+ygm);

	return output;
}