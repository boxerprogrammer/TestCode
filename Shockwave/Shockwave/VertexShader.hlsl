#include"ShaderHeader.hlsli"

struct VsInput//IAから渡される頂点情報
{
    float4 pos : POSITION0;//通常の座標★
    float4 spos : POSITION1;//補助座標
    float4 norm : NORMAL;//法線ベクトル
    float4 tan : TANGENT;//接線ベクトル
    float4 binorm : BINORMAL;//従法線ベクトル
    float4 col : COLOR0;//カラー★
    float4 spc : COLOR1;//スペキュラー
    float2 uv : TEXCOORD0;//UV★
    float2 suv : TEXCOORD1;//SubUV
};

cbuffer TransFormation : register(b0)
{
    matrix mat;//座標変換行列
};

PsInput main( VsInput input )
{
    PsInput ret;
    ret.pos = mul(mat, input.pos);
    ret.col = input.col;
    ret.uv = input.uv;
	return ret;
}