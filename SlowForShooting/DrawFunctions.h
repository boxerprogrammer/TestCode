#pragma once
#include<tchar.h>

//�����p���C�u�����p���O���
namespace my {
/// <summary>
/// �摜�����[�h����
/// </summary>
/// <param name="path">�p�X������</param>
/// <returns>�摜�̃n���h��</returns>
int MyLoadGraph(const TCHAR* path);

int MyDrawRectRotaGraph(int x, int y,
						int left, int top,
						int width, int height,
						float scale, float angle,
						int handle, bool transFlg);
int MyDrawRectRotaGraph(float x, float y,
	int left, int top,
	int width, int height,
	float scale, float angle,
	int handle, bool transFlg);

}