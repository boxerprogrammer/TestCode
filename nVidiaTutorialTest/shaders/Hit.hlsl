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

/*
	uint vertId = 3*PrimitiveIndex();
	hitColor = 	BTriVertex[vertId+0].color*brycent.x+
						BTriVertex[vertId+1].color*brycent.y+
						BTriVertex[vertId+2].color*brycent.z;
						*/
	float3 hitColor=float3(0.6,0.7,0.6);
	uint iid=InstanceID();
	
	if(0<=iid&&iid<4){
		//hitColor=cA[iid]*brycent.x+cB[iid]*brycent.y+cC[iid]*brycent.z;
		hitColor=cA*brycent.x+cB*brycent.y+cC*brycent.z;
	}
	
	/*					
	switch(InstanceID()){
		case 0:
			hitColor=A*brycent.x+B*brycent.y+C*brycent.z;
			break;
		case 1:
			hitColor=B*brycent.x+B*brycent.y+C*brycent.z;
			break;
		case 2:
			hitColor=C*brycent.x+B*brycent.y+C*brycent.z;
			break;
	}
	*/
	//float3 hitColor=A*brycent.x+B*brycent.y+C*brycent.z;
 	payload.colorAndDistance = float4(hitColor, RayTCurrent());
}
