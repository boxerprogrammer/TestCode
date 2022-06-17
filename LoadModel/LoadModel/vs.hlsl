struct VertexInput {
	float3 pos : POSITION;//���W
	float3 norm : NORMAL;//�@��
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
	float3 tangent:TANGENT;//�ڃx�N�g��
	float3 binormal:BINORMAL;//�]�@���x�N�g��
};
//struct VertexInput
//{
//	float3 pos			: POSITION;	        // ���W(���[�J�����)
//	float3 norm			: NORMAL;	        // �@��(���[�J�����)
//	float4 diffuse			: COLOR0;		// �f�B�t���[�Y�J���[
//	float4 specular			: COLOR1;		// �X�y�L�����J���[
//	float4 uv0		: TEXCOORD0;		// �e�N�X�`�����W
//	float4 uv1		: TEXCOORD1;		// �T�u�e�N�X�`�����W
//	float3 tan			: TANGENT0;	        // �ڐ�(���[�J�����)
//	float3 binorm			: BINORMAL0;		// �]�@��(���[�J�����)
//	int4   blendIndices0	        : BLENDINDICES0;	// �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
//	float4 blendWeight0		: BLENDWEIGHT0;		// �X�L�j���O�����p�E�G�C�g�l
//};

cbuffer BaseCBuffer : register(b1) {
	matrix AntiViewportM;//4x4�r���[�|�[�g�t�s��
	matrix ProjectionM;//4x4�v���W�F�N�V�����s��
	float4x3 ViewM;//4x3(�r���[�s��)
	float4x3 LocalM;//4x3(��]�g�k���s�ړ�)
	float4		ToonOutLineSize;						// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;							// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;						// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;						// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;//�l�ߕ�(����)
}

struct VSOutput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
	float2 uv:TECOORD;
	float3 col:COLOR;
};

VSOutput main(VertexInput input)
{
	VSOutput ret;
	float4 pos = float4(input.pos,1);
	pos.xyz = mul(pos, LocalM);//���[���h
	pos.xyz = mul(pos, ViewM);//�r���[
	pos = mul(pos, ProjectionM);//�v���W�F�N�V����

	//�@���ɂ���]�����͏�Z���悤��
	float3 norm=mul(input.norm, LocalM);
	float3 tan = mul(input.tangent, LocalM);
	float3 bin = mul(input.binormal, LocalM);
	
	ret.svpos = pos;
	ret.pos = input.pos;
	ret.uv = input.uv0.xy;
	ret.norm = normalize(norm);
	ret.tan = normalize(tan);
	ret.bin = normalize(bin);
	ret.col = input.diffuse.rgb;
	return ret;
}