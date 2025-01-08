// 頂点シェーダーの出力
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
    float3 Normal : NORMAL;
    float3 pos : POSITION;
};

struct ForDecalCBuff
{
    float minY;
    float maxY;
    float radius;
    float dummy[1];
};
cbuffer ConstatnBuffer : register(b3)
{
    ForDecalCBuff g_decal;
}

struct Axis3D
{
    float3 right;
    float3 up;
    float3 front;
};

struct DecalBox
{
    float3 pos;
    int enable;
    float3 size;
    float dummy1;
    float3 right;
    float dummy2;
    float3 up;
    float dummy3;
    float3 front;
    float dummy4;
};
cbuffer ConstatnBuffer : register(b4)
{
    DecalBox g_decalBox[8];
}

SamplerState smp : register(s0); 
Texture2D tex : register(t3);


float4 main(PS_INPUT psinput) : SV_TARGET
{
    float3 uvw=0;
    float4 decal = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (g_decalBox[i].enable)
        {
            float3 vec = psinput.pos - g_decalBox[i].pos;
            uvw = float3(dot(vec, g_decalBox[i].right), dot(vec, g_decalBox[i].up), dot(vec, g_decalBox[i].front));
        
            uvw /= g_decalBox[i].size;

            if (abs(uvw.x) < 1.0 && abs(uvw.y) < 1.0 && abs(uvw.z) < 1.0)
            {
                uvw.xy += 1.0;
                uvw.xy /= 2.0;
                decal = tex.Sample(smp, uvw.xy);
                break;
            }
        }
    }
    if (decal.a == 0)
    {
        decal.rgb = 1.0f;
    }
    
    float bright = max(saturate(dot(-float3(1, -1, 1), psinput.Normal)), 0.2);
	return float4(decal.rgb*bright, 1.0);
}