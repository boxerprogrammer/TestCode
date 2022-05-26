// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
	float4 Specular        : COLOR1 ;		// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;	// テクスチャ座標
	float4 svPosition        : SV_POSITION;	// 座標( プロジェクション空間 )
	float4 Position : POSITION0;
	float4 P : POSITION1;
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0           : SV_TARGET0 ;	// 色
} ;


// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor ;			// アルファ値等

	float		MulAlphaColor ;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		AlphaTestRef ;			// アルファテストで使用する比較値
	float2		Padding1 ;

	int			AlphaTestCmpMode ;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		Padding2 ;

	float4		IgnoreTextureColor ;	// テクスチャカラー無視処理用カラー
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register( b1 )
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base ;
} ;


SamplerState g_DiffuseMapSampler            : register( s0 ) ;		// ディフューズマップテクスチャ
Texture2D    g_DiffuseMapTexture            : register( t0 ) ;		// ディフューズマップテクスチャ
Texture2D    pattern            : register(t1);
cbuffer OreStruct: register(b0) {
	float time;
	float threshold;
}

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float b = pattern.Sample(g_DiffuseMapSampler, PSInput.TexCoords0);
	float  alp = (PSInput.Position.y - threshold) / 200.0;
	float alb=alp + b;

	if (alb > 0.0) {
		discard;
	}
	alb=saturate(1.0 - abs(alb));
	float bloom=pow(alb+(1-alb)*b, 20)*2;



	// テクスチャカラーの読み込み
	//TextureDiffuseColor =  g_DiffuseMapTexture.Sample( g_DiffuseMapSampler, PSInput.TexCoords0 ) ;

	// 出力カラー = テクスチャカラー
	//PSOutput.Color0 = TextureDiffuseColor ;

	// 出力アルファ = テクスチャアルファ * 不透明度
	//PSOutput.Color0.a = TextureDiffuseColor.a * g_Base.FactorColor.a ;
	PSOutput.Color0 =  float4(PSInput.Diffuse.rgb+float3(bloom,bloom*0.5,bloom*0.5),1);
	// 出力パラメータを返す
	return PSOutput ;
}

