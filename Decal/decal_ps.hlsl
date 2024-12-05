// �`�悷��e�N�X�`��
SamplerState smp : register(s0);
Texture2D dst : register(t0);
Texture2D src : register(t1);

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 dif : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer buff : register(b4)
{
    float2 uvpos;
    float angle;
    float dummy;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    //���x�N�g��
    float2 i = float2(cos(angle), sin(angle));
    float2 j = float2(-sin(angle), cos(angle));
    
    float2 brushSize;
    float2 srcSize;
    src.GetDimensions(srcSize.x, srcSize.y);
    dst.GetDimensions(brushSize.x, brushSize.y);
    float2 p;
    float2 rate = brushSize / srcSize;
    p = uvpos.xy + float2(input.uv - float2(0.5, 0.5)) * rate; //���ɂȂ��Ă�摜��UV���W�擾
    float2 uv = float2(dot((p - uvpos.xy) / rate, i), dot((p - uvpos.xy) / rate, j)) + float2(0.5, 0.5);
    
    float4 srcCol = src.Sample(smp, p);
    if (srcCol.a==0)
    {
        discard;
    }
    float4 dstCol = dst.Sample(smp, uv);
    return lerp(srcCol, dstCol, dstCol.a);
}