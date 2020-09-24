#include "Common.hlsl"

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
/*
cbuffer Colors : register(b0){
	MyStructColor Tint[3];
}

cbuffer Colors : register(b0){
	float3 cA[3];
	float3 cB[3];
	float3 cC[3];
}
*/
cbuffer Colors : register(b0){
	float3 cA;
	float3 cB;
	float3 cC;
}

[shader("closesthit")] 
void PlaneClosestHit(inout HitInfo payload, Attributes attrib) {
	float3 barycentrics=float3(1.0f-attrib.bary.x-attrib.bary.y,attrib.bary.x,attrib.bary.y);
	float3 hitColor=float3(0.7f,0.7f,0.3f);
	payload.colorAndDistance=float4(hitColor,RayTCurrent());
}

[shader("closesthit")] 
void ClosestHit(inout HitInfo payload, Attributes attrib) 
{

	float3 brycent=float3(1.0f-attrib.bary.x-attrib.bary.y,attrib.bary.x,attrib.bary.y);
	const float3 A=float3(1,0,0);
	const float3 B=float3(0,1,0);
	const float3 C=float3(0,0,1);

	float3 hitColor=float3(0.6,0.7,0.6);
	uint iid=InstanceID();
	
	if(0<=iid&&iid<4){
		hitColor=cA*brycent.x+cB*brycent.y+cC*brycent.z;
	}

 	payload.colorAndDistance = float4(hitColor, RayTCurrent());
}
