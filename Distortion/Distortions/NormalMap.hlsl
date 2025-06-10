SamplerState smp : register(s0);
Texture2D tex : register(t0);
Texture2D normTex : register(t1);

//CPUから渡された定数バッファ
cbuffer ConstantBuffer : register(b4)
{
    float time;
    float3 dummy;
};


struct PsInput
{
    float4 pos : SV_Position;
    float4 col : COLOR0;
    //float4 spc : COLOR1;
    float2 uv : TEXCOORD;
};

//法線マップシェーダ
float4 main(PsInput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);//元の色を取ってくる
    float3 norm = normTex.Sample(smp, input.uv).xyz;//法線情報を取ってくる
    //法線情報(色情報)をベクトルの法線情報に変換する。ついでに正規化も
    norm = normalize(norm * float3(2, 2, -2) + float3(-1, -1, 1));
    
    //ライトベクトルの定義
    float theta = (time + 1.0) * 3.1415926535;
    float3 light = normalize(float3(cos(theta), sin(theta), 1));
     //ディフューズ(拡散反射)成分を計算(内積をとる)
    float diffuse = saturate(dot(-light, norm));
    
    
    float3 V = float3(0, 0, 1);//視線ベクトル
    float3 R = reflect(light,norm);//反射ベクトル
    float specular = pow(saturate(dot(R, -V)), 20.0)*1.0;
    
    float ambient = 0.35;
    
   
    //diffuseにもとの色を乗算して出力する
    return float4(max(saturate(col.rgb * diffuse + specular), ambient * col.rgb), col.a);
}