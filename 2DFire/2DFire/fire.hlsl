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
	float4 overcolor           : SV_TARGET1;	// 過剰色
};

//定数バッファ
cbuffer buff: register(b0) {
	float uscale;//ディゾルブの閾値
	float angle;//光の角度
	float voffset;
	//float uscale;
}

SamplerState sam;//サンプラ(後述)
Texture2D tex:register(t0);//テクスチャ
Texture2D distortion:register(t1);//歪み
Texture2D stencil:register(t2);//ステンシル
						   //1ピクセル1ピクセル塗りつぶしの際に呼ばれる
PS_OUTPUT main(PS_INPUT input) {
	PS_OUTPUT output;

	float2 uv = input.uv;
	uv = fmod(input.uv *float2(uscale,1.0),1.0);

	//歪みマップ
	float3 dist = distortion.Sample(sam, fmod(uv + float2(0, voffset), 1.0)).rgb;//nx,ny,nz
	dist = (dist * 2) - 1;
	dist = normalize(dist);
	dist.z = -dist.z;
	float2 distuv = uv + (dist * 0.05).xy;

	float opaque = stencil.Sample(sam, distuv).r;
	opaque *= stencil.Sample(sam, distuv*float2(1/uscale/2,0.0)).r;
	float omopaque = 1 - saturate(opaque);
	if (omopaque < 0.33) {
		discard;
	}

	float4 col = tex.Sample(sam, distuv);
	col = col+opaque*float4(5.0f,0.0f,0.0f,1.0f)+(pow(opaque,2)*5)*float4(1,0.5,0,1)+pow(opaque,10)*100;
	output.color = col;
	if (col.a == 0.0) {
		discard;//なかったことにするよ
	}
	output.overcolor = float4(step(0.99,col.rgb),col.a);// , float4(0.1, 0.1, 0.1, 0.1));

	return output;
}