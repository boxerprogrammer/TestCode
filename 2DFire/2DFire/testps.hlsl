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
Texture2D tex;//テクスチャ
//1ピクセル1ピクセル塗りつぶしの際に呼ばれる
PS_OUTPUT main(PS_INPUT input) {
	PS_OUTPUT output;
	output.color = float4(input.uv, 0.5, 1);//赤を返す
	return output;
}