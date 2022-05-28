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

//�萔�o�b�t�@
cbuffer buff: register(b0) {
	float threshold;//�f�B�]���u��臒l
	float angle;//���̊p�x
}

SamplerState sam;//�T���v��(��q)
Texture2D tex:register(t0);//�e�N�X�`��
Texture2D ptn:register(t1);//�p�^�[��
Texture2D norm:register(t2);//�@���}�b�v
						   //1�s�N�Z��1�s�N�Z���h��Ԃ��̍ۂɌĂ΂��
PS_OUTPUT main(PS_INPUT input) {
	PS_OUTPUT output;
	float discval = ptn.Sample(sam, input.uv).r;
	float yedge= pow(1.0 - saturate(abs(threshold - input.uv.y)),5);
	if (input.uv.y +(yedge*discval) < threshold) {
		discard;
	}
	//�m�[�}���}�b�v
	float3 light = float3(cos(angle), -sin(angle), 0.5);
	light = normalize(light);
	float3 normal = norm.Sample(sam, input.uv).rgb;//nx,ny,nz
	normal = (normal * 2) - 1;
	normal = normalize(normal);
	normal.z = -normal.z;

	float bright = saturate(saturate(dot(-light, normal)) + 0.25);

	float4 col = tex.Sample(sam,input.uv);// float4(input.uv, 0.5, 1);//�Ԃ�Ԃ�
	output.color = col;
	if (col.a == 0.0) {
		discard;//�Ȃ��������Ƃɂ����
	}
	//float edge = 1.0-saturate(abs(threshold - discval));
	float edge = saturate((1.0 - abs(input.uv.y + (yedge * discval) - threshold)));
	edge = pow(edge, 15);
	//output.color.rgb = 1.0 - col.rgb;
	/*float m = fmod(input.uv.x+input.uv.y, 0.1)*10;*/
	output.color.rgb = float3(0, 1, 1) * edge +
		(output.color.rgb*bright) * (1.0 - edge);
	return output;
}