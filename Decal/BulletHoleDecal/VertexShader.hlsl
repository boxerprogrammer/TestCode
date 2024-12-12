// 頂点シェーダーの入力
struct VS_INPUT
{
    float3 Position : POSITION; // 座標( ローカル空間 )
    float3 Normal : NORMAL; // 法線( ローカル空間 )
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float4 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 TexCoords1 : TEXCOORD1; // サブテクスチャ座標
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
    float3 Normal : NORMAL;
    float3 pos : POSITION;
};


// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
    float4x4 AntiViewportMatrix; // アンチビューポート行列
    float4x4 ProjectionMatrix; // ビュー　→　プロジェクション行列
    float4x3 ViewMatrix; // ワールド　→　ビュー行列
    float4x3 LocalWorldMatrix; // ローカル　→　ワールド行列

    float4 ToonOutLineSize; // トゥーンの輪郭線の大きさ
    float DiffuseSource; // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float SpecularSource; // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float MulSpecularColor; // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float Padding;
};

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
    float4 ShadowMapLightViewProjectionMatrix[3][4]; // シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
    float4 TextureMatrix[3][2]; // テクスチャ座標操作用行列
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE g_Base;
};

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
{
    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX g_OtherMatrix;
};

struct ForDecalCBuff
{
    float minY;
    float maxY;
    float radius;
    float dummy[1];
};

VS_OUTPUT main( VS_INPUT input )
{
    VS_OUTPUT vsout;
    
    float4 pos = float4(input.Position, 1);
    pos = float4(mul(pos, g_Base.LocalWorldMatrix), 1.0);
    pos = float4(mul(pos, g_Base.ViewMatrix), 1.0);
    pos = float4(mul(pos, g_Base.ProjectionMatrix));
    float4 normal = float4(input.Normal, 0);
    normal = float4(mul(normal, g_Base.LocalWorldMatrix), 0);
    normal = float4(mul(normal, g_Base.ViewMatrix), 0);
    normal = normalize(normal);
    vsout.pos = input.Position;
    vsout.Position = pos;
    vsout.Diffuse = float4(1, 1, 1, 1);
    vsout.Specular = float4(1, 1, 1, 1);
    vsout.TexCoords0 = float2(1, 1);
    vsout.Normal = normal;
	return vsout;
}