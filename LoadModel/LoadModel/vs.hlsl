struct VertexInput {
	float3 pos : POSITION;//座標
	float3 norm : NORMAL;//法線
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
	float3 tangent:TANGENT;//接ベクトル
	float3 binormal:BINORMAL;//従法線ベクトル
};
//struct VertexInput
//{
//	float3 pos			: POSITION;	        // 座標(ローカル空間)
//	float3 norm			: NORMAL;	        // 法線(ローカル空間)
//	float4 diffuse			: COLOR0;		// ディフューズカラー
//	float4 specular			: COLOR1;		// スペキュラカラー
//	float4 uv0		: TEXCOORD0;		// テクスチャ座標
//	float4 uv1		: TEXCOORD1;		// サブテクスチャ座標
//	float3 tan			: TANGENT0;	        // 接線(ローカル空間)
//	float3 binorm			: BINORMAL0;		// 従法線(ローカル空間)
//	int4   blendIndices0	        : BLENDINDICES0;	// スキニング処理用 Float型定数配列インデックス
//	float4 blendWeight0		: BLENDWEIGHT0;		// スキニング処理用ウエイト値
//};

cbuffer BaseCBuffer : register(b1) {
	matrix AntiViewportM;//4x4ビューポート逆行列
	matrix ProjectionM;//4x4プロジェクション行列
	float4x3 ViewM;//4x3(ビュー行列)
	float4x3 LocalM;//4x3(回転拡縮平行移動)
	float4		ToonOutLineSize;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;//詰め物(無視)
}

struct VSOutput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
	float2 uv:TECOORD;
	float3 col:COLOR;
};

VSOutput main(VertexInput input)
{
	VSOutput ret;
	float4 pos = float4(input.pos,1);
	pos.xyz = mul(pos, LocalM);//ワールド
	pos.xyz = mul(pos, ViewM);//ビュー
	pos = mul(pos, ProjectionM);//プロジェクション

	//法線にも回転成分は乗算しようね
	float3 norm=mul(input.norm, LocalM);
	float3 tan = mul(input.tangent, LocalM);
	float3 bin = mul(input.binormal, LocalM);
	
	ret.svpos = pos;
	ret.pos = input.pos;
	ret.uv = input.uv0.xy;
	ret.norm = normalize(norm);
	ret.tan = normalize(tan);
	ret.bin = normalize(bin);
	ret.col = input.diffuse.rgb;
	return ret;
}