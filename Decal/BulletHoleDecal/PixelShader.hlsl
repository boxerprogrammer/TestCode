// ���_�V�F�[�_�[�̏o��
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 Position : SV_POSITION; // ���W( �v���W�F�N�V������� )
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

SamplerState smp : register(s0); 
Texture2D tex : register(t3);


float4 main(PS_INPUT psinput) : SV_TARGET
{
    const float pi = 3.1415926535;
    float2 uv;
    //�~���}�b�s���O
    float2 pos = psinput.pos.xz;
    pos = normalize(pos);
    uv.x = (atan2(pos.y, pos.x) + pi) / (pi * 2.0);
    uv.y = (g_decal.maxY - psinput.pos.y) / (g_decal.maxY - g_decal.minY);
    
    //���̃}�b�v
    //float3 pos = normalize(psinput.pos);
    //float xzradius = length(pos.xz);
    //float theta = atan2(pos.z, pos.x) + pi; //���������p�x
    //float phi = (pi / 2.0) - atan2(pos.y, xzradius); //���������p�x
    //uv.x = theta / (pi * 2.0);
    //uv.y = phi / pi;
    
    
    float4 decal = tex.Sample(smp, uv);
    if (decal.a == 0 || uv.y<0.0 || uv.y > 1.0)
    {
        decal.rgb = 1.0f;
    }
    
    float bright = max(saturate(dot(-float3(1, -1, 1), psinput.Normal)), 0.2);
	return float4(decal.rgb*bright, 1.0);
}