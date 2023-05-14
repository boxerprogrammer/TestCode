#include<DxLib.h>
#include<cassert>
#include"DrawFunctions.h"

namespace my {
/// <summary>
/// �O���t�B�N�X�����[�h����
/// </summary>
/// <param name="path">�摜�t�@�C���̃p�X</param>
/// <returns>�摜�̃n���h��</returns>
int MyLoadGraph(const TCHAR* path) {
	int handle = LoadGraph(path);
	assert(handle >= 0);
	return handle;
}
int 
MyDrawRectRotaGraph(int x, int y, int left, int top, int width, int height, float scale, float angle, int handle, bool transFlg)
{
	return DrawRectRotaGraph(x, y,
							left, top,
							width, height,
							scale, angle,
							handle, transFlg);
}

int
MyDrawRectRotaGraph(float x, float y, int left, int top, int width, int height, float scale, float angle, int handle, bool transFlg)
{
	return DrawRectRotaGraph(
		static_cast<int>(x), static_cast<int>(y),
		left, top,
		width, height,
		scale, angle,
		handle, transFlg);
}

}//end of namespace my