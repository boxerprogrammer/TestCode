#include "Common.hlsl"

struct ShadowHitInfo{
	bool isHit;
};

struct STriVertex{
	float3 vertex;
	float4 color;
};
struct MyStructColor{
	float4 a;
	float4 b;
	float4 c;
};

StructuredBuffer<STriVertex> BTriVertex : register(t0);
StructuredBuffer<int> indices : register(t1);
RaytracingAccelerationStructure SceneBVH : register(t2);

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
	
	float factor = shadowPayload.isHit?0.3f:1.0f;
	
	float3 barycentrics=float3(1.0f-attrib.bary.x-attrib.bary.y,attrib.bary.x,attrib.bary.y);
	float3 hitColor=float3(0.7f,0.7f,0.3f)*0.5+payload.colorAndDistance.rgb*0.5;
	payload.colorAndDistance=float4(hitColor*factor,RayTCurrent());
}

[shader("closesthit")] 
void ClosestHit(inout HitInfo payload, Attributes attrib) 
{

	float3 brycent=float3(1.0f-attrib.bary.x-attrib.bary.y,attrib.bary.x,attrib.bary.y);


	float3 hitColor=float3(0.6,0.7,0.6);
	uint iid=InstanceID();
	
	if(iid<3){
		hitColor=cA*brycent.x+cB*brycent.y+cC*brycent.z;
	}
	uint vertId = 3 * PrimitiveIndex();
	//hitColor = cA;
	float3 nV = BTriVertex[indices[vertId + 0]].color * brycent.x +
		BTriVertex[indices[vertId + 1]].color * brycent.y +
		BTriVertex[indices[vertId + 2]].color * brycent.z;

	float3 lightPos = float3(2, 2, -2);
	lightPos = normalize(lightPos);
	nV = RotVectorByQuat(nV, cB);
	float b = dot(lightPos, nV);

 	payload.colorAndDistance = float4(b*cA, RayTCurrent());
}
