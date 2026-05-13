#include "PixelShader.h"


// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// ディフューズカラー
	float4 Specular        : COLOR1 ;		// スペキュラカラー
	float4 TexCoords0_1    : TEXCOORD0 ;	// xy:テクスチャ座標 zw:サブテクスチャ座標
	float3 VPosition       : TEXCOORD1 ;	// 座標( ビュー空間 )
	float3 VNormal         : TEXCOORD2 ;	// 法線( ビュー空間 )
#ifdef BUMPMAP
	float3 VTan            : TANGENT ;    // 接線( ビュー空間 )
	float3 VBin            : BINORMAL ;    // 従法線( ビュー空間 )
#endif // BUMPMAP
	float2 Fog             : TEXCOORD5 ;	// フォグパラメータ( x )   高さフォグパラメータ( y )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// シャドウマップ０のライト座標( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// シャドウマップ１のライト座標( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// シャドウマップ２のライト座標( x, y, z )
#endif // SHADOWMAP
    float4 Position : SV_POSITION;
    float3 WorldPosition : POSITION0;
    float4 PositionD : POSITION1; //デプス検証用
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{

	float4 Color0          : SV_TARGET0 ;	// 色
	float4 Normal          : SV_TARGET1 ;	// 法線( ビュー空間 )
	float4 Depth           : SV_TARGET2 ;	// 深度
    float4 Distance        : SV_TARGET3; // 距離
    

} ;

#if BUMPMAP
SamplerState g_NormalMapSampler             : register( s1 ) ;		// 法線マップテクスチャ
Texture2D    g_NormalMapTexture             : register( t1 ) ;		// 法線マップテクスチャ
#endif // BUMPMAP

#if USE_SPETEX
SamplerState g_SpecularMapSampler           : register( s2 ) ;		// スペキュラマップテクスチャ
Texture2D    g_SpecularMapTexture           : register( t2 ) ;		// スペキュラマップテクスチャ
#endif // USE_SPETEX

#if TOON
SamplerState g_ToonDiffuseGradSampler       : register( s3 ) ;		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ
Texture2D    g_ToonDiffuseGradTexture       : register( t3 ) ;		// トゥーンレンダリング用ディフューズカラーグラデーションテクスチャ

SamplerState g_ToonSpecularGradSampler      : register( s4 ) ;		// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ
Texture2D    g_ToonSpecularGradTexture      : register( t4 ) ;		// トゥーンレンダリング用スペキュラカラーグラデーションテクスチャ

#if TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
SamplerState g_ToonSphereMapSampler         : register( s5 ) ;		// トゥーンレンダリング用スフィアマップテクスチャ
Texture2D    g_ToonSphereMapTexture         : register( t5 ) ;		// トゥーンレンダリング用スフィアマップテクスチャ
#endif // TOON_SPHEREOP_MUL || TOON_SPHEREOP_ADD
SamplerState g_ToonRGBtoVMaxRGBVolumeSampler: register( s6 ) ;		// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
Texture3D    g_ToonRGBtoVMaxRGBVolumeTexture: register( t6 ) ;		// トゥーンレンダリング用RGB輝度飽和処理用ボリュームテクスチャ
#endif // TOON

#if SHADOWMAP
SamplerState g_ShadowMap0Sampler            : register( s8  ) ;		// シャドウマップ０テクスチャ
Texture2D    g_ShadowMap0Texture            : register( t8  ) ;		// シャドウマップ０テクスチャ

SamplerState g_ShadowMap1Sampler            : register( s9  ) ;		// シャドウマップ１テクスチャ
Texture2D    g_ShadowMap1Texture            : register( t9  ) ;		// シャドウマップ１テクスチャ

SamplerState g_ShadowMap2Sampler            : register( s10 ) ;		// シャドウマップ２テクスチャ
Texture2D    g_ShadowMap2Texture            : register( t10 ) ;		// シャドウマップ２テクスチャ
#endif


#define SHADOWMAP0_ENABLE_LGT0 g_ShadowMap.Data[ 0 ].Enable_Light0
#define SHADOWMAP0_ENABLE_LGT1 g_ShadowMap.Data[ 0 ].Enable_Light1
#define SHADOWMAP0_ENABLE_LGT2 g_ShadowMap.Data[ 0 ].Enable_Light2
#define SHADOWMAP0_ENABLE_LGT3 0.0f
#define SHADOWMAP0_ENABLE_LGT4 0.0f
#define SHADOWMAP0_ENABLE_LGT5 0.0f

#define SHADOWMAP1_ENABLE_LGT0 g_ShadowMap.Data[ 1 ].Enable_Light0
#define SHADOWMAP1_ENABLE_LGT1 g_ShadowMap.Data[ 1 ].Enable_Light1
#define SHADOWMAP1_ENABLE_LGT2 g_ShadowMap.Data[ 1 ].Enable_Light2
#define SHADOWMAP1_ENABLE_LGT3 0.0f
#define SHADOWMAP1_ENABLE_LGT4 0.0f
#define SHADOWMAP1_ENABLE_LGT5 0.0f

#define SHADOWMAP2_ENABLE_LGT0 g_ShadowMap.Data[ 2 ].Enable_Light0
#define SHADOWMAP2_ENABLE_LGT1 g_ShadowMap.Data[ 2 ].Enable_Light1
#define SHADOWMAP2_ENABLE_LGT2 g_ShadowMap.Data[ 2 ].Enable_Light2
#define SHADOWMAP2_ENABLE_LGT3 0.0f
#define SHADOWMAP2_ENABLE_LGT4 0.0f
#define SHADOWMAP2_ENABLE_LGT5 0.0f

cbuffer BoundingBox : register(b4)
{
    float4 vmin;
    float4 vmax;
};



Texture2D tex : register(t0);
Texture2D dissolveTex : register(t4);
Texture2D matCapTex : register(t5);
SamplerState smp : register(s0);



// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT	PSOutput ;
    PSOutput.Normal.x = (PSInput.VNormal.x);

    PSOutput.Normal.y = (PSInput.VNormal.y);

    PSOutput.Normal.z = (PSInput.VNormal.z);
    PSOutput.Normal.xyz = normalize(PSOutput.Normal.xyz);
	PSOutput.Normal.w = 1.0 ;
    
    PSOutput.Distance.x = length(PSInput.VPosition)/1000.0f;
    PSOutput.Distance.w = 1.0;
    float matcapRate = 0.0f;
#ifdef BUMPMAP
    float3 normalMap=g_NormalMapTexture.Sample(g_NormalMapSampler,PSInput.TexCoords0_1.xy).xyz;
    normalMap = normalMap*2.0-1.0;
    PSOutput.Normal.xyz= PSInput.VBin*normalMap.x + 
                        PSInput.VTan*normalMap.y + 
                        PSInput.VNormal*normalMap.z;
    PSOutput.Normal.xyz = normalize(PSOutput.Normal.xyz);
    matcapRate = 1.0f;
#endif
    
    //ラスタライザがやってるようにもとのZ値で割ってあげます。
    //割ってあげないと
    //far(z-near)/(far-near)になっているため、範囲が0.0～1.0にならなくなっています。
    //このため、本来のZで割るために本来のZが入っているwで割ってあげます
    PSOutput.Depth.r = PSInput.PositionD.z / PSInput.PositionD.w;//0～1
    PSOutput.Depth.a = 1.0;//a=0.0にすると、αテストではじかれるので、1.0を入れておく

    float3 bbUVW0_1 = (PSInput.WorldPosition - vmin.xyz) / (vmax.xyz-vmin.xyz);
	
    float dissolve = dissolveTex.Sample(smp, bbUVW0_1.xz).r;
    
    float threshold = vmax.w+dissolve * 0.25;
    float param = PSOutput.Depth.r - threshold; //bbUVW0_1.y - threshold;
    
    if (param>0)
    {
        discard;
    }
	
    float bright = max(saturate(dot(normalize(-g_Common.Light[0].Direction), PSOutput.Normal.xyz)),
				g_Common.Material.Ambient_Emissive + g_Common.Light[0].Ambient);
	
    float3 eyeRay = normalize(-PSInput.VPosition);
    float3 ref = reflect(g_Common.Light[0].Direction, PSOutput.Normal.xyz);
    float spec = pow(saturate(dot(eyeRay, ref)), g_Common.Material.Power);
	
    float4 texCol = tex.Sample(smp, PSInput.TexCoords0_1.xy);
    float4 color = g_Common.Material.Diffuse * texCol*bright + spec;
	
    float edge = pow(1.0 - saturate(abs(param)), 50);
    float4 edgeColor = float4(0, 1, 1, 1);//エッジカラー水色
    
    float2 matcapUV = normalize(PSInput.VNormal.xyz).xy * 0.5 + 0.5;
    float4 matcapColor = matCapTex.Sample(smp, matcapUV);
	
    PSOutput.Color0 = lerp(lerp(float4(color.rgb, g_Common.Material.Diffuse.a), edgeColor, edge), matcapColor, matcapRate);
	return PSOutput ;
}

