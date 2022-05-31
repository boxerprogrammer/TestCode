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
Texture2D tex:register(t0);//�e�N�X�`��
Texture2D norm:register(t1);//�e�N�X�`��
cbuffer ConstantBuffer : register(b0) {
	float footstep;
	float angle;
}
PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float2 n=norm.Sample(sam, 0.25*(0.5+input.uv.y)*input.uv+float2(angle*0.025+ footstep,0)).xy;
	n = n * 2 - 1;//-1�`1
	float3 nm = normalize(float3(n,-0.5));
	float3 light = float3(1, -1, 5);
	float3 ray = float3((input.uv - 0.5) * 2,1);
	ray = normalize(ray);
	ray=reflect(ray, nm);
	light = normalize(-light);
	float b = saturate(dot(light, ray));
	float s = pow(b, 50);
	
	float2 ygm = n.xy*0.1 ;
	ygm = float2(cos(angle) * ygm.x + sin(angle) * ygm.y,
		-sin(angle) * ygm.x + cos(angle) * ygm.y);
	output.color = tex.Sample(sam, input.uv+ygm*0.5);
	output.color.rgb *= dot(light,nm)*0.75;
	output.color.rgb += float3(0.5,0.5,1.0)*s*0.25;
	//output.color.rgb += b;
	return output;
}