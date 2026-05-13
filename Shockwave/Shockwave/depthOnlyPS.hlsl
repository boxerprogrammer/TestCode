#include "shader/PixelShader.h"

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
    float4 Depth : SV_TARGET0; // 深度
};

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    //ラスタライザがやってるようにもとのZ値で割ってあげます。
    //割ってあげないと
    //far(z-near)/(far-near)になっているため、範囲が0.0～1.0にならなくなっています。
    //このため、本来のZで割るために本来のZが入っているwで割ってあげます
    PSOutput.Depth.r = PSInput.PositionD.z / PSInput.PositionD.w; //0～1
    PSOutput.Depth.a = 1.0; //a=0.0にすると、αテストではじかれるので、1.0を入れておく

    return PSOutput;
}

