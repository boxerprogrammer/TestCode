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

//定数バッファ
cbuffer buff: register(b0) {
	float threshold;//ディゾルブの閾値
	float angle;//光の角度
}

SamplerState sam;//サンプラ(後述)
Texture2D tex:register(t0);//テクスチャ
Texture2D ptn:register(t1);//パターン
Texture2D norm:register(t2);//法線マップ
						   //1ピクセル1ピクセル塗りつぶしの際に呼ばれる
PS_OUTPUT main(PS_INPUT input) {
	PS_OUTPUT output;
	float discval = ptn.Sample(sam, input.uv).r;
	float yedge= pow(1.0 - saturate(abs(threshold - input.uv.y)),5);
	if (input.uv.y +(yedge*discval) < threshold) {
		discard;
	}
	//ノーマルマップ
	float3 light = float3(cos(angle), -sin(angle), 0.5);
	light = normalize(light);
	float3 normal = norm.Sample(sam, input.uv).rgb;//nx,ny,nz
	normal = (normal * 2) - 1;
	normal = normalize(normal);
	normal.z = -normal.z;

	float bright = saturate(saturate(dot(-light, normal)) + 0.25);

	float4 col = tex.Sample(sam,input.uv);// float4(input.uv, 0.5, 1);//赤を返す
	output.color = col;
	if (col.a == 0.0) {
		discard;//なかったことにするよ
	}
	//float edge = 1.0-saturate(abs(threshold - discval));
	float edge = saturate((1.0 - abs(input.uv.y + (yedge * discval) - threshold)));
	edge = pow(edge, 15);
	//output.color.rgb = 1.0 - col.rgb;
	/*float m = fmod(input.uv.x+input.uv.y, 0.1)*10;*/
	output.color.rgb = float3(0, 1, 1) * edge +
		(output.color.rgb*bright) * (1.0 - edge);
	return output;
}