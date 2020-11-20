struct Buffer_t {
	float grpId;
	float grpThrdId;
	float dspThrdId;
	uint groupIndex;
};

RWStructuredBuffer< Buffer_t> OutBuff:register(u0);

//id���������ނ����̃v���O�����ł�
[numthreads(4, 4, 4)]
void main(uint3 groupId : SV_GroupID,
	uint3 groupThreadId : SV_GroupThreadID,
	uint3 dispatchThreadId : SV_DispatchThreadID,
	uint groupIndex : SV_GroupIndex)
{
	//��������ID����
	OutBuff[dispatchThreadId.x * 4 * 4 + dispatchThreadId.y * 4 + dispatchThreadId.z].dspThrdId =
		dispatchThreadId.x * 4 * 4 + dispatchThreadId.y * 4 + dispatchThreadId.z;
	OutBuff[groupId.x * 2 * 2 + groupId.y * 2 + groupId.z].grpId =
		groupId.x * 2 * 2 + groupId.y * 2 + groupId.z;
	OutBuff[groupThreadId.x * 2 * 2 + groupThreadId.y * 2 + groupThreadId.z].grpThrdId =
		groupThreadId.x * 2 * 2 + groupThreadId.y * 2 + groupThreadId.z;
	OutBuff[groupIndex].groupIndex = groupIndex;
}