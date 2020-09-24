struct ShadpwHitInfo{
	bool isHit;
};

strucgt Attributes{
	float2 uv;
};


[shader("closesthit")] 
void ShadowClosestHit(inout ShadowHitInfo hit : SV_Raypayload) {
	hit.isHit=true;
}

[shader("miss")]
void ShadowMiss(intout ShadowHitInfo hit : SV_Raypayload){
	hit.isHit=false;
}


