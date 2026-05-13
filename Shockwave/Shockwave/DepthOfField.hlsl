#include"ShaderHeader.hlsli"
Texture2D tex : register(t0);//通常テクスチャ
Texture2D depth : register(t1);//深度値
Texture2D blur : register(t2);//ぼけた画像
SamplerState smp : register(s0);

cbuffer ClickedPos : register(b5)
{
    float4 clickedpos;
};

struct PSInput
{
    float4 pos : SV_Position;
    float4 dif : COLOR0;
    float4 spc : COLOR1;
    float2 uv : TEXCOORD0;
    float2 suv : TEXCOORD1;
};

float4 main(PSInput input) : SV_TARGET
{
    uint2 wh;
    uint dim;
    tex.GetDimensions(0, wh.x, wh.y, dim);//画面の解像度を得る
    //クリックした場所のUV値を得る
    float2 clickedUV = clickedpos.xy / (float2) wh;
    
    float4 col = tex.Sample(smp, input.uv);//現在のUV部分のピクセル色
    float4 blurCol = blur.Sample(smp, input.uv);//ぼけ画像のピクセル色
    float dep = depth.Sample(smp, input.uv).r;//現在のUVの深度値
    
    float clickedDep = depth.Sample(smp, clickedUV);//クリックした場所の深度値
    
    //threshold(閾値)の値が大きければ大きいほどよくぼける
    //クリックしたところの深度から離れれば離れるほどぼかしたい
    
    //clickedpos.wは「被写界深度」(必ずくっきり見える範囲)
    //step関数というのは、第二引数のほうが大きければ1、そうでなければ0
    //を返す関数。(x,y){ return x>0:0?1 }
    //この結果0か1になりますが、さいごにthresholdがかけられている
    //ため、結果が台形になります。
 #ifdef USE_DOF
    float threshold = distance(clickedDep, dep);
    threshold = step(clickedpos.w, threshold) * threshold;

    if (clickedpos.z == 0.0)
    {
        threshold = 0.0;
    }
    return lerp(col, blurCol, threshold);
#else
 #ifdef USE_FOG
    float threshold = saturate(dep-clickedDep);
    return lerp(col, float4(1,1,1,1), threshold);
#else
    return col;
#endif
#endif
    
}