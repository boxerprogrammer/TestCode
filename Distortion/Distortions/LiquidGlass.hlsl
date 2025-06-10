Texture2D tex : register(t0); //前のレンダリング結果
Texture2D tex2 : register(t1); //前のレンダリング結果
SamplerState smp : register(s0);
struct PsInput
{
    float4 pos : SV_Position;
    float4 col : COLOR0;
    //float4 spc : COLOR1;
    float2 uv : TEXCOORD0;
    float2 suv : TEXCOORD1;
};

float4 main(PsInput input) : SV_TARGET
{
    float2 vec2 = (input.uv - float2(0.5f, 0.5))*2; // UV座標を-0.5～0.5に変換
    float3 vec = float3(vec2, sqrt(1.0 - saturate(dot(vec2, vec2)))); // 2Dベクトルを3Dに変換
    float len = length(vec2); // ベクトルの長さを計算
    if (len > 1.0)
    {
        discard;
    }
    else
    {
        len = pow(saturate(len), 4); // ベクトルの長さを0-1に変換
    }

    float2 scale = float2(100.0 / 640.0, 100.0 / 480.0); // UV座標のスケール
    vec = normalize(-vec)*len; // ベクトルを正規化
    float2 uv = input.suv+vec.xy*scale; // UV座標を0-1に変換
    float4 col = tex.Sample(smp, uv); // 前のレンダリング結果を取得
    float4 col2 = tex2.Sample(smp, uv); // 前のレンダリング結果を取得
    
    return float4(lerp(col.rgb, col2.rgb, pow((1 - length(vec2)),2)), col2.a);
}