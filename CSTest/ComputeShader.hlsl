struct Buffer_t {
	float grpId;
	float grpThrdId;
	float dspThrdId;
	uint groupIndex;
};

//CPU側のデータ型と合わせる必要がある。
struct SimpleBuffer_t
{
	int		i;
	float	f;
};

StructuredBuffer<SimpleBuffer_t>	inBuff0 : register(t0);
StructuredBuffer<SimpleBuffer_t>	inBuff1 : register(t1);

RWStructuredBuffer< Buffer_t> outBuff:register(u0);

//idを書き込むだけのプログラムです
[numthreads(4, 4, 4)]
void main(uint3 groupId : SV_GroupID,
	uint3 groupThreadId : SV_GroupThreadID,
	uint3 dispatchThreadId : SV_DispatchThreadID,
	uint groupIndex : SV_GroupIndex)
{
	float angle = 3.14f * (float)groupIndex / 32.0f;
	//ただただIDを代入
	outBuff[dispatchThreadId.x * 4 * 4 + dispatchThreadId.y * 4 + dispatchThreadId.z].dspThrdId =
		dispatchThreadId.x * 4 * 4 + dispatchThreadId.y * 4 + dispatchThreadId.z* inBuff0[groupIndex].f;
	outBuff[groupId.x * 2 * 2 + groupId.y * 2 + groupId.z].grpId =
		groupId.x * 2 * 2 + groupId.y * 2 + groupId.z;
	outBuff[groupThreadId.x * 2 * 2 + groupThreadId.y * 2 + groupThreadId.z].grpThrdId =
		groupThreadId.x * 2 * 2 + groupThreadId.y * 2 + (float)groupThreadId.z*cos(angle)+sin(angle);
	outBuff[groupIndex].groupIndex = inBuff0[groupIndex].i*groupIndex;

}