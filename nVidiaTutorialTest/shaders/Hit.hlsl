#include "Common.hlsl"

struct ShadowHitInfo{
	bool isHit;
};

struct STriVertex{
	float3 pos;
	float3 normal;
	float2 uv;
};
struct MyStructColor{
	float4 a;
	float4 b;
	float4 c;
};

StructuredBuffer<STriVertex> BTriVertex : register(t0);
//struct VerexAndMaterialID {
//	int vertId;
//	float material;
//};
StructuredBuffer<int> indices : register(t1);
RaytracingAccelerationStructure SceneBVH : register(t2);
StructuredBuffer<int> mtlTbl : register(t3);
StructuredBuffer<float3> materials : register(t4);


cbuffer Colors : register(b0){
	float3 cA;
	float4 cB;
	float3 cC;
}

// クオータニオンから回転ベクトルを求める
float3 RotVectorByQuat(float3 v, float4 q)
{
	return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}


[shader("closesthit")] 
void PlaneClosestHit(inout HitInfo payload, Attributes attrib) {
	
	float3 lightPos=float3(2,2,-2);
	float3 worldOrigin = WorldRayOrigin()+RayTCurrent()*WorldRayDirection();//平面上の現在の座標
	
	float3 lightDir=normalize(lightPos-worldOrigin);//その座標から見たライトまでのベクトル
	
	RayDesc ray;
	ray.Origin = worldOrigin;
	ray.Direction = lightDir;
	ray.TMin=0.01f;
	ray.TMax=100000.0f;
	bool hit=true;
	
	ShadowHitInfo shadowPayload;
	shadowPayload.isHit=false;
	
	TraceRay(SceneBVH,RAY_FLAG_NONE,0xff,1,0,1,ray,shadowPayload);
	
	float3 rray = reflect(WorldRayDirection(), float3(0, 1, 0));
	rray = normalize(rray);
	ray.Origin = worldOrigin;
	ray.Direction = rray;
	ray.TMin = 0.01f;
	ray.TMax = 100000.0f;
	TraceRay(SceneBVH, RAY_FLAG_NONE, 0xff, 0, 0, 1, ray, payload);
	
	float shadowFactor = shadowPayload.isHit?0.3f:1.0f;
	
	float3 brycent=float3(1.0f-attrib.bary.x-attrib.bary.y,
		attrib.bary.x,
		attrib.bary.y);
	uint vertId = 3 * PrimitiveIndex();
	STriVertex v0 = BTriVertex[vertId+0];
	STriVertex v1 = BTriVertex[vertId + 1];
	STriVertex v2 = BTriVertex[vertId + 2];
	//float2 uv = v0.uv * brycent.x + v1.uv * brycent.y+v2.uv*brycent.z;
	float2 uv = v0.uv + attrib.bary.x * (v1.uv - v0.uv) + attrib.bary.y * (v2.uv - v0.uv);
	float3 col = float3(0, 0, 0);
	if (fmod(uv.x + uv.y, 1.f) < 0.5) {
		col = float3(0, 0.8,0.0);
	} 
	col = float3(v0.normal.xyz);
	float3 hitColor=col*0.5+payload.colorAndDistance.rgb*0.5;//反射ブレンド
	payload.colorAndDistance=float4(hitColor* shadowFactor,RayTCurrent());
}

[shader("closesthit")] 
void ClosestHit(inout HitInfo payload, Attributes attrib) 
{

	float3 brycent=float3(1.0f-attrib.bary.x-attrib.bary.y,attrib.bary.x,attrib.bary.y);

	float3 hitColor=float3(0.6,0.7,0.6);
	uint iid=InstanceID();
	
	//if(iid<3){
	//	hitColor=cA*brycent.x+cB*brycent.y+cC*brycent.z;
	//}
	uint vertId = 3 * PrimitiveIndex();
	//hitColor = cA;
	STriVertex v0 = BTriVertex[indices[vertId + 0]];
	STriVertex v1 = BTriVertex[indices[vertId + 1]];
	STriVertex v2 = BTriVertex[indices[vertId + 2]];
	float3 nV = v0.normal * brycent.x +
		v1.normal * brycent.y +
		v2.normal * brycent.z;

	float3 lightPos = float3(2, 2, -2);
	lightPos = normalize(lightPos);
	nV = RotVectorByQuat(nV, cB);
	float b = saturate(dot(lightPos, nV)+0.5);
	uint idx = mtlTbl[PrimitiveIndex()] % 14;
	float3 p = materials[idx];
	//float2 uv = v0.uv * brycent.x + v1.uv * brycent.y + v2.uv * brycent.z;
	float2 uv = v0.uv + brycent.x * (v1.uv - v0.uv) + brycent.y * (v2.uv - v0.uv);
	float3 col = float3(0, 0, 0);
	if (fmod(uv.x + uv.y, 0.08f) < 0.04) {
		col = float3(0, 0.8, 0.0);
	}
 	payload.colorAndDistance = float4(b*p, RayTCurrent());
}
