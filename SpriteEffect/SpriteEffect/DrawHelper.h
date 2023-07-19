#pragma once
#include<array>
#include<vector>
void MyDrawGraph(int x, int y, int imgHandle, int secondImg, int thirdImg, int psHandle);
void MyDrawRotaGraph(int x, int y, float scale,float angle,int imgHandle, int secondImg, int thirdImg, int psHandle);
void MyDrawRotaGraph2(int x, int y, int cx,int cy,float scale, float angle, int imgHandle, int secondImg, int thirdImg, int psHandle);
void MyDrawExtendGraph(int x1, int y1, int x2, int y2, int imgHandle, int secondImg, int thirdImg, int psHandle);
std::vector<int> LoadSequentialGraph(const wchar_t* baseFolderPath, const wchar_t* baseName, int pictNum);