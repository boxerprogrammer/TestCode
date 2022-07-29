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
	float4 overcolor           : SV_TARGET1;	// �ߏ�F
};

//�萔�o�b�t�@
cbuffer buff: register(b0) {
	float uscale;//�f�B�]���u��臒l
	float angle;//���̊p�x
	float voffset;
	//float uscale;
}

SamplerState sam;//�T���v��(��q)
Texture2D tex:register(t0);//�e�N�X�`��
Texture2D distortion:register(t1);//�c��
Texture2D stencil:register(t2);//�X�e���V��
						   //1�s�N�Z��1�s�N�Z���h��Ԃ��̍ۂɌĂ΂��
PS_OUTPUT main(PS_INPUT input) {
	PS_OUTPUT output;

	float2 uv = input.uv;
	uv = fmod(input.uv *float2(uscale,1.0),1.0);

	//�c�݃}�b�v
	float3 dist = distortion.Sample(sam, fmod(uv + float2(0, voffset), 1.0)).rgb;//nx,ny,nz
	dist = (dist * 2) - 1;
	dist = normalize(dist);
	dist.z = -dist.z;
	float2 distuv = uv + (dist * 0.05).xy;

	float opaque = stencil.Sample(sam, distuv).r;
	opaque *= stencil.Sample(sam, distuv*float2(1/uscale/2,0.0)).r;
	float omopaque = 1 - saturate(opaque);
	if (omopaque < 0.33) {
		discard;
	}

	float4 col = tex.Sample(sam, distuv);
	col = col+opaque*float4(5.0f,0.0f,0.0f,1.0f)+(pow(opaque,2)*5)*float4(1,0.5,0,1)+pow(opaque,10)*100;
	output.color = col;
	if (col.a == 0.0) {
		discard;//�Ȃ��������Ƃɂ����
	}
	output.overcolor = float4(step(0.99,col.rgb),col.a);// , float4(0.1, 0.1, 0.1, 0.1));

	return output;
}