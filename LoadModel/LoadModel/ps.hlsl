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
	matrix AntiViewportM;//4x4ビューポート逆行列
	matrix ProjectionM;//4x4プロジェクション行列
	float4x3 ViewM;//4x3(ビュー行列)
	float4x3 LocalM;//4x3(回転拡縮平行移動)
	float4		ToonOutLineSize;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;//詰め物(無視)
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

	float3 light = float3(1,-1,1);//光ベクトル
	//float3 cpos = float3(0, 0, -300);// float3(-dot(ViewM[0], ViewM[3]), -dot(ViewM[1], ViewM[3]), -dot(ViewM[2], ViewM[3]));
	float3 ray = input.pos-cameraPos;//視線ベクトル
	ray = normalize(ray);


	light=normalize(light);

	float3 nmap = norm.Sample(sam, input.uv);
	nmap = nmap * 2 - 1;

	float3 nm = normalize(nmap);
	
	float3 n=
		input.norm+
		normalize(
		(nm.x*input.tan)+//tanが接空間のXベクトル
		(nm.y*input.bin)+//binが接空間のYベクトル
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