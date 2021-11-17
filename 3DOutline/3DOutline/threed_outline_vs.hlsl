// 頂点シェーダーの入力
struct VS_INPUT
{
	float3 Position        : POSITION ;			// 座標( ローカル空間 )
	float3 Normal          : NORMAL ;			// 法線( ローカル空間 )
	float4 Diffuse         : COLOR0 ;			// ディフューズカラー
	float4 Specular        : COLOR1 ;			// スペキュラカラー
	float4 TexCoords0      : TEXCOORD0 ;		// テクスチャ座標
	float4 TexCoords1      : TEXCOORD1 ;		// サブテクスチャ座標
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
	float4 Specular        : COLOR1 ;		// スペキュラカラー
	float2 TexCoords0      : TEXCOORD0 ;	// テクスチャ座標
	float4 Position        : SV_POSITION ;	// 座標( プロジェクション空間 )
} ;


// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4x4		AntiViewportMatrix;				// アンチビューポート行列
	float4x4		ProjectionMatrix;					// ビュー　→　プロジェクション行列
	float4x3		ViewMatrix;						// ワールド　→　ビュー行列
	float4x3		LocalWorldMatrix;					// ローカル　→　ワールド行列

	float4		ToonOutLineSize ;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource ;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource ;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor ;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding ;
} ;

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[ 3 ][ 4 ] ;			// シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
	float4		TextureMatrix[ 3 ][ 2 ] ;								// テクスチャ座標操作用行列
} ;

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE				: register( b1 )
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base ;
} ;

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX			: register( b2 )
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix ;
} ;



// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	float3 light=normalize(float3(1, 1, 1));
	VS_OUTPUT VSOutput ;
	float4 lLocalPosition ;
	float4 lWorldPosition ;
	float4 lViewPosition ;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標のセット
	lLocalPosition.xyz = VSInput.Position;
	lLocalPosition.w = 1.0f ;

	// 座標計算( ローカル→ビュー→プロジェクション )
	lWorldPosition = float4(mul(lLocalPosition,g_Base.LocalWorldMatrix ), 1.0f);

	lViewPosition = float4(mul(lWorldPosition,g_Base.ViewMatrix ), 1.0f);

	VSOutput.Position = mul(lViewPosition, g_Base.ProjectionMatrix);

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	VSOutput.TexCoords0 =  VSInput.TexCoords0;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )
	VSOutput.Diffuse=VSInput.Diffuse*(max(saturate(dot(VSInput.Normal,light)),0.3));
	VSOutput.Specular= VSInput.Specular;

	// 出力パラメータを返す
	return VSOutput ;
}

