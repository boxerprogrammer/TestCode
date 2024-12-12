// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
    float3 Position : POSITION; // ���W( ���[�J����� )
    float3 Normal : NORMAL; // �@��( ���[�J����� )
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float4 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 TexCoords1 : TEXCOORD1; // �T�u�e�N�X�`�����W
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 Position : SV_POSITION; // ���W( �v���W�F�N�V������� )
    float3 Normal : NORMAL;
    float3 pos : POSITION;
};


// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
    float4x4 AntiViewportMatrix; // �A���`�r���[�|�[�g�s��
    float4x4 ProjectionMatrix; // �r���[�@���@�v���W�F�N�V�����s��
    float4x3 ViewMatrix; // ���[���h�@���@�r���[�s��
    float4x3 LocalWorldMatrix; // ���[�J���@���@���[���h�s��

    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding;
};

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
    float4 ShadowMapLightViewProjectionMatrix[3][4]; // �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
    float4 TextureMatrix[3][2]; // �e�N�X�`�����W����p�s��
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE g_Base;
};

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
{
    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX g_OtherMatrix;
};

struct ForDecalCBuff
{
    float minY;
    float maxY;
    float radius;
    float dummy[1];
};

VS_OUTPUT main( VS_INPUT input )
{
    VS_OUTPUT vsout;
    
    float4 pos = float4(input.Position, 1);
    pos = float4(mul(pos, g_Base.LocalWorldMatrix), 1.0);
    pos = float4(mul(pos, g_Base.ViewMatrix), 1.0);
    pos = float4(mul(pos, g_Base.ProjectionMatrix));
    float4 normal = float4(input.Normal, 0);
    normal = float4(mul(normal, g_Base.LocalWorldMatrix), 0);
    normal = float4(mul(normal, g_Base.ViewMatrix), 0);
    normal = normalize(normal);
    vsout.pos = input.Position;
    vsout.Position = pos;
    vsout.Diffuse = float4(1, 1, 1, 1);
    vsout.Specular = float4(1, 1, 1, 1);
    vsout.TexCoords0 = float2(1, 1);
    vsout.Normal = normal;
	return vsout;
}