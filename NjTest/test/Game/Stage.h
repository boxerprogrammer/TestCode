#pragma once
#include<tchar.h>
#include<vector>
#include<memory>
#include"../Geometry.h"
struct StageHeader {
	char id[4];
	unsigned int size;
	unsigned int mapW;
	unsigned int mapH;
	unsigned char chipW;
	unsigned char chipH;
	unsigned char layerCount;
	unsigned char bitCount;//8固定
};
class Camera;
/// <summary>
/// ステージデータ読み込み
/// およびデータの管理を行う
/// </summary>
class Stage
{
private:
	using StageLayerData_t = std::vector<unsigned char>;
	std::vector<StageLayerData_t> stagedata_;
	StageHeader header_;
	std::shared_ptr<Camera> camera_;
public:
	Stage(std::shared_ptr<Camera> c);
	void Load(const TCHAR* path);
	void Update();
	void BackDraw();
	void DrawChips(size_t layerNo);
	void FrontDraw();
	Size GetStageSize()const;
};

