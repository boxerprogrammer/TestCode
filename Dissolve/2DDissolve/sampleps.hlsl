// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 dif         : COLOR0;		// �f�B�t���[�Y�J���[
	float4 spc        : COLOR1;		// �X�y�L�����J���[
	float2 uv      : TEXCOORD0;	// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color           : SV_TARGET0;	// �F
};
SamplerState sam;//�T���v��(��q)
Texture2D tex;//�e�N�X�`��
//1�s�N�Z��1�s�N�Z���h��Ԃ��̍ۂɌĂ΂��
PS_OUTPUT main(PS_INPUT input) {
	PS_OUTPUT output;
	output.color = float4(input.uv, 0.5, 1);//�Ԃ�Ԃ�
	return output;
}