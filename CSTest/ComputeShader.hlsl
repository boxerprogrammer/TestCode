struct Buffer_t {
	float val;
};

RWStructuredBuffer< Buffer_t> OutBuff:register(u0);

//id���������ނ����̃v���O�����ł�
[numthreads(4, 4, 4)]
void main( uint3 id : SV_DispatchThreadID )
{
	//��������ID����
	OutBuff[id.x * 4 * 4 + id.y * 4 + id.z].val = id.x * 4 * 4 + id.y * 4 + id.z;
}