cbuffer ConstantBuffer : register( b0 )
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VSInput
{
    float3 pos : POSITION0; // 座標
    float4 spos : POSITION1; // 補助座標
    float3 norm : NORMAL; // 法線
    float3 tan : TANGENT; // 接線
    float3 binorm : BINORMAL; // 従法線
    float4 col : COLOR0; // ディフューズカラー
    float4 spc : COLOR1; // スペキュラカラー
    float2 uv : TEXCOORD0; // テクスチャ座標
    float2 suv : TEXCOORD1; // 補助テクスチャ座標
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 pos : POSITION;
    float4 norm : NORMAL;
    
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

VSOutput main( VSInput input )
{
    VSOutput output;
    float4 pos = mul(world, float4(input.pos, 1.0f));
    float3 norm = mul(world, float4(input.norm, 1.0f));
    pos = mul(view,pos);
    output.pos = pos;
    pos = mul(projection,pos);
    output.svpos = pos;
    output.norm = float4(normalize(norm), 0);
    output.col = input.col;
    output.uv = input.uv;
	return output;
}