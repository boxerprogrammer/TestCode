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
	vector<StageLayerData_t> localStageData(header_.layerCount);
	auto layerSize = header_.mapW * header_.mapH;
	for (int i = 0; i < header_.layerCount; ++i) {
		stagedata_[i].resize(layerSize);
		localStageData[i].resize(layerSize);
		DxLib::FileRead_read(localStageData[i].data(), layerSize, h);
	}
	DxLib::FileRead_close(h);
	//�ϊ����
	for (int d = 0; d < header_.layerCount; ++d) {
		for (size_t y = 0; y < header_.mapH; ++y) {
			for (size_t x = 0; x < header_.mapW; ++x) {
				stagedata_[d][y + x * header_.mapH] = localStageData[d][x + y * header_.mapW];
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
	const int xoffset = camera_->ViewOffset().x;
	//��ŏ��������邪�܂��͕�����₷�����̂܂ܕ��ׂ�(�A���g��͂���)
	for (size_t x = 0; x < header_.mapW; ++x) {
		for (size_t y = 0; y < header_.mapH; ++y) {
			auto chip = stagedata_[layerNo][y + x * header_.mapH];//�`�b�v�ԍ��擾
			DrawRectRotaGraph2(
				xoffset+x * header_.chipW * draw_scale, //�\��X���W
				yoffset + y * header_.chipH * draw_scale,//�\��Y���W
				(chip % 16) * header_.chipW,//���G�̍����W
				(chip / 16) * header_.chipH,//���G�̏���W
				header_.chipW,//���G�̕�
				header_.chipH, //���G�̍���
				0, 0,//���㒆�S�ɂ��Ă���
				draw_scale,//�g�嗦
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