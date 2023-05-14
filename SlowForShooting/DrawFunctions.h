#pragma once
#include<tchar.h>

//自分用ライブラリ用名前空間
namespace my {
/// <summary>
/// 画像をロードする
/// </summary>
/// <param name="path">パス文字列</param>
/// <returns>画像のハンドル</returns>
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