#include "PixelShader.h"

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
    float4 Specular : COLOR1; // スペキュラカラー
    float4 TexCoords0_1 : TEXCOORD0; // xy:テクスチャ座標 zw:サブテクスチャ座標
    float3 VPosition : TEXCOORD1; // 座標( ビュー空間 )
    float3 VNormal : TEXCOORD2; // 法線( ビュー空間 )
#ifdef BUMPMAP
	float3 VTan            : TANGENT ;    // 接線( ビュー空間 )
	float3 VBin            : BINORMAL ;    // 従法線( ビュー空間 )
#endif // BUMPMAP
    float2 Fog : TEXCOORD5; // フォグパラメータ( x )   高さフォグパラメータ( y )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// シャドウマップ０のライト座標( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// シャドウマップ１のライト座標( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// シャドウマップ２のライト座標( x, y, z )
#endif // SHADOWMAP
    float4 Position : SV_POSITION;
    float3 WorldPosition : POSITION0;
    float4 PositionD : POSITION1; //デプス検証用
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // 色
    float4 Normal : SV_TARGET1; // 法線( ビュー空間 )
    float4 Depth : SV_TARGET2; // 深度
    float4 Distance : SV_TARGET3; // 距離
    
};
Texture2D tex : register(t0);
Texture2D dissolveTex : register(t4);
Texture2D matCapTex : register(t5);
SamplerState smp : register(s0);

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    PSOutput.Normal.x = (PSInput.VNormal.x);
    PSOutput.Normal.y = (PSInput.VNormal.y);
    PSOutput.Normal.z = (PSInput.VNormal.z);
    PSOutput.Normal.xyz = normalize(PSOutput.Normal.xyz);
    PSOutput.Normal.w = 1.0;
    PSOutput.Distance.x = length(PSInput.VPosition) / 1000.0f;
    PSOutput.Distance.w = 1.0;
    PSOutput.Depth.r = PSInput.PositionD.z / PSInput.PositionD.w; //0～1
    PSOutput.Depth.a = 1.0; //a=0.0にすると、αテストではじかれるので、1.0を入れておく

    float bright = max(saturate(dot(normalize(-g_Common.Light[0].Direction), PSOutput.Normal.xyz)),
				g_Common.Material.Ambient_Emissive + g_Common.Light[0].Ambient);
	
    float3 eyeRay = normalize(-PSInput.VPosition);
    float3 ref = reflect(g_Common.Light[0].Direction, PSOutput.Normal.xyz);
    float spec = pow(saturate(dot(eyeRay, ref)), g_Common.Material.Power);
	
    
    float2 matcapUV = PSInput.VNormal.xy * 0.5 + 0.5;
    float4 matcapColor = matCapTex.Sample(smp, matcapUV);
    
    
    float4 texCol = tex.Sample(smp, PSInput.TexCoords0_1.xy);
    float4 color = g_Common.Material.Diffuse * texCol * bright + spec;
    
    float rim = pow(1.0 - dot(eyeRay, PSOutput.Normal.xyz), 2.0)*2.0;
    float4 rimColor = float4(0.9, 1.0, 1.0, 1.0);
    
    //PSOutput.Color0 = rim*rimColor;//
    PSOutput.Color0 = float4(lerp(color.rgb, matcapColor.rgb, 0.5), 1);
    return PSOutput;
}

