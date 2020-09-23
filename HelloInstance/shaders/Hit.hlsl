#include "Common.hlsl"

struct STriVertex{
	float3 vertex;
	float4 color;
};

//cbuffer Colors : register(b0)
StructuredBuffer<STriVertex> BTriVertex : register(t0);

[shader("closesthit")] 
void ClosestHit(inout HitInfo payload, Attributes attrib) 
{

	float3 brycent=float3(1.0f-attrib.bary.x-attrib.bary.y,attrib.bary.x,attrib.bary.y);
	const float3 A=float3(1,0,0);
	const float3 B=float3(0,1,0);
	const float3 C=float3(0,0,1);

	uint vertId = 3*PrimitiveIndex();
	float3 hitColor = 	BTriVertex[vertId+0].color*brycent.x+
						BTriVertex[vertId+1].color*brycent.y+
						BTriVertex[vertId+2].color*brycent.z;
	hitColor=float3(0.7,0.7,0.7);
						
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
	
	//float3 hitColor=A*brycent.x+B*brycent.y+C*brycent.z;
 	payload.colorAndDistance = float4(hitColor, RayTCurrent());
}
