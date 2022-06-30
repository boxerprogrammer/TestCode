struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
	float2 uv:TECOORD;
	float3 col:COLOR;
};

cbuffer CBuffer : register(b0) {
	float4 cameraPos;
	matrix vmat;
}
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
SamplerState sam:register(s0);
Texture2D<float4> tex:register(t0);
Texture2D<float4> norm:register(t1);
Texture2D<float4> rough:register(t2);
Texture2D<float4> metallic:register(t3);
Texture2D<float4> sph:register(t4);

float4 main(PSInput input) : SV_TARGET
{
	float rgh= rough.Sample(sam,input.uv).r;

	float3 light = float3(1,-1,1);//���x�N�g��
	//float3 cpos = float3(0, 0, -300);// float3(-dot(ViewM[0], ViewM[3]), -dot(ViewM[1], ViewM[3]), -dot(ViewM[2], ViewM[3]));
	float3 ray = input.pos-cameraPos;//�����x�N�g��
	ray = normalize(ray);


	light=normalize(light);

	float3 nmap = norm.Sample(sam, input.uv);
	nmap = nmap * 2 - 1;

	float3 nm = normalize(nmap);
	
	float3 n=
		input.norm+
		normalize(
		(nm.x*input.tan)+//tan���ڋ�Ԃ�X�x�N�g��
		(nm.y*input.bin)+//bin���ڋ�Ԃ�Y�x�N�g��
		(nm.z*input.norm)//
		);
	n = normalize(n);

	float fresnel = 1 - saturate(dot(-ray, n));

	float b = saturate(dot(n, -light));
	b = max(b, 0.25);

	float3 ref=reflect(light, n);
	float spec=pow(saturate(dot(-ray, ref)),20);

	float3 refRay = reflect(ray, n);
	refRay = mul(refRay, vmat);
	//refRay = normalize(refRay);
	refRay.xy = refRay.xy*float2(0.4,-0.4) + 0.5;
	float4 spCol = sph.Sample(sam, refRay.xy);

	float3 col = tex.Sample(sam,input.uv);
	//col *= input.col;
	//rgh *= fresnel;

	float met = metallic.Sample(sam, input.uv);
	

	float3 rgb = lerp(col * b,spec, rgh);

	rgh *= (1 - fresnel);

	float3 refCol = lerp(spCol.rgb, rgb, rgh);

	return float4(rgb*refCol , 1.0f);
}