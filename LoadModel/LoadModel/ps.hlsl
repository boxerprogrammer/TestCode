struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
	float2 uv:TECOORD;
	float3 col:COLOR;
};

SamplerState sam:register(s0);
Texture2D<float4> tex:register(t0);

Texture2D<float4> norm:register(t1);

float4 main(PSInput input) : SV_TARGET
{
	float3 light = float3(1,-1,1);//光ベクトル
	float3 ray = float3(0, 0, 1);//視線ベクトル
	light=normalize(light);

	float3 nmap = norm.Sample(sam, input.uv);
	nmap = nmap * 2 - 1;

	float3 nm = normalize(nmap);
	
	float3 n=
		input.norm+
		normalize(
		(nmap.x*input.tan)+//tanが接空間のXベクトル
		(nmap.y*input.bin)+//binが接空間のYベクトル
		(nmap.z*input.norm)//
		);
	n = normalize(n);

	float b = saturate(dot(n, -light));
	b = max(b, 0.5);

	float3 ref=reflect(light, n);
	float spec=pow(saturate(dot(-ray, ref)),20);

	float3 col = tex.Sample(sam,input.uv);
	col *= input.col;
	return float4(col*b+spec , 1.0f);
}