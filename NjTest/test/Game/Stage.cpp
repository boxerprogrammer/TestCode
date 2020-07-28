#include "Stage.h"
#include"../Geometry.h"
#include"Camera.h"
#include<DxLib.h>
#include<cassert>

using namespace std;

namespace {
	int stageAtlasH_ = -1;
	constexpr float draw_scale = 2.0f;
	constexpr int ground_line = 600;
}
Stage::Stage(std::shared_ptr<Camera> c) : camera_(c) {
	stageAtlasH_ = LoadGraph(L"Resource/Image/Background/Level/level1_atlas.png");
	assert(stageAtlasH_ >= 0);
}
void 
Stage::Load(const TCHAR* path) {
	auto h=DxLib::FileRead_open(path);
	assert(h > 0);
	DxLib::FileRead_read(&header_, sizeof(header_), h);
	
	stagedata_.resize(header_.layerCount);
	vector<StageLayerData_t> rawStageData(header_.layerCount);
	auto layerSize = header_.mapW * header_.mapH;
	for (int i = 0; i < header_.layerCount; ++i) {
		stagedata_[i].resize(layerSize);
		rawStageData[i].resize(layerSize);
		DxLib::FileRead_read(rawStageData[i].data(), layerSize, h);
	}
	DxLib::FileRead_close(h);
	//縦横変換作業
	for (int d = 0; d < header_.layerCount; ++d) {
		for (size_t y = 0; y < header_.mapH; ++y) {
			for (size_t x = 0; x < header_.mapW; ++x) {
				stagedata_[d][y + x * header_.mapH] = rawStageData[d][x + y * header_.mapW];
			}
		}
	}
}

void 
Stage::Update() {

}
void 
Stage::BackDraw() {
	for (int i = 0; i < header_.layerCount-1; ++i) {
		DrawChips(i);
	}
}

void Stage::DrawChips(size_t layerNo)
{
	
	const int yoffset = ground_line - ((int)header_.chipH * draw_scale * header_.mapH);
	const int xoffset =  camera_->ViewOffset().x;
	auto rc = camera_->GetViewRange();
	//後で書き換えるがまずは分かりやすくそのまま並べる(但し拡大はする)
	for (size_t x = 0; x < header_.mapW; ++x) {
		int xpos = x * header_.chipW * draw_scale;
		int xmargin = header_.chipW * draw_scale;
		if (xpos < rc.Left()- xmargin ||  rc.Right()+ xmargin<xpos) {
			continue;
		}
		for (size_t y = 0; y < header_.mapH; ++y) {
			auto chip = stagedata_[layerNo][y + x * header_.mapH];//チップ番号取得
			DrawRectRotaGraph2(
				xoffset+x * header_.chipW * draw_scale, //表示X座標
				yoffset + y * header_.chipH * draw_scale,//表示Y座標
				(chip % 16) * header_.chipW,//元絵の左座標
				(chip / 16) * header_.chipH,//元絵の上座標
				header_.chipW,//元絵の幅
				header_.chipH, //元絵の高さ
				0, 0,//左上中心にしている
				draw_scale,//拡大率
				0.0f,
				stageAtlasH_, true);
		}
	}
}

void
Stage::FrontDraw() {
	DrawChips(header_.layerCount - 1);
}

Size 
Stage::GetStageSize()const {
	return Size(header_.mapW * header_.chipW * draw_scale, header_.mapH * header_.chipH * draw_scale);
}