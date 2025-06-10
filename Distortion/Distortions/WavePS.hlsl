Texture2D tex : register(t0);//前のレンダリング結果
SamplerState smp : register(s0);

//CPUから渡された定数バッファ
cbuffer ConstantBuffer : register(b4)
{
    float time;
    float x;//衝撃波の中心座標X
    float y;//衝撃波の中心座標Y
    float r;//半径
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
    uint2 wh;
    uint levels;
    //画面全体の幅と高さを得る
    tex.GetDimensions(0, wh.x, wh.y, levels);
    float2 rate = float2(wh.x, wh.y);//画面サイズを割合とする
    
    //現在のピクセル座標-クリック座標
    float2 vec = input.uv * rate - float2(x, y);
    //距離を測る
    float dis = length(vec);
    
    vec = normalize(vec);//方向だけを使いたい
    
    vec *=  pow(1.0-saturate(distance(dis, r) / r), 2);
    float4 col = tex.Sample(smp, input.uv-vec*0.05);
    return col;
}