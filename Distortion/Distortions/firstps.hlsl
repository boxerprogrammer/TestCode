Texture2D tex : register(t0);
Texture2D dissolveTex : register(t1);
SamplerState smp : register(s0);
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

float4 main(PsInput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);
    float dis = dissolveTex.Sample(smp, input.uv).r;
    float value = saturate(time + dis);
    if (value < input.uv.y || col.a==0)
    {
        discard;//そのピクセルの値を返さずに計算を破棄します
    }
    //ちょうど消える近辺を強調したい
    //1.0 - saturate(value - input.uv.y)←消える近辺が１になる
    //これを20乗することで、エッジ近辺が1で、その周辺から
    //急激に0に近づくグラフになります
    float edge = pow(1.0 - saturate(value - input.uv.y), 20.0);
    
    return float4(lerp(col, float4(0.2, 0.75, 1.0,1.0), edge));
}