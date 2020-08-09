#include "Stage.h"
#include"../Geometry.h"
#include"Camera.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>
#include"../System/Debugger.h"
#include<limits>
#include<array>
#include"../System/File.h"
#include"../System/FileManager.h"

using namespace std;

namespace {
	
	constexpr float block_scale = 2.0f;
	constexpr int ground_line = 600;
	constexpr float naraku_y = FLT_MAX;
	constexpr uint8_t layer_no_back0 = 0;//�w�i(��)
	constexpr uint8_t layer_no_back1 = 1;//�w�i(��O)
	constexpr uint8_t layer_no_front = 2;//�O�i(�v���C���[�̑O)
	constexpr uint8_t layer_no_collision = 3;//�n�`�R���W����
	constexpr uint8_t layer_no_enemy = 4;//�G�z�u(255�̓{�X)
}
Stage::Stage(std::shared_ptr<Camera> c) : camera_(c) {
	auto& fileMgr = FileManager::Instance();
	stageAtlasH_ = fileMgr.Load(L"Resource/Image/Background/Level/level1_atlas.png")->Handle();
	assert(stageAtlasH_ >= 0);
}
void 
Stage::Load(const TCHAR* path) {
	auto& fileMgr=FileManager::Instance();
	auto file=fileMgr.Load(path);
	file->CopyRead(header_);

	stagedata_.resize(header_.layerCount);
	vector<StageLayerData_t> rawStageData(header_.layerCount);
	auto layerSize = header_.mapW * header_.mapH;
	for (int i = 0; i < header_.layerCount; ++i) {
		stagedata_[i].resize(layerSize);
		rawStageData[i].resize(layerSize);
		file->CopyRead(rawStageData[i].data(), layerSize);
	}
	fileMgr.Delete(file);

	//�c���ϊ����
	for (int d = 0; d < header_.layerCount; ++d) {
		for (size_t y = 0; y < header_.mapH; ++y) {
			for (size_t x = 0; x < header_.mapW; ++x) {
				stagedata_[d][y + x * header_.mapH] = rawStageData[d][x + y * header_.mapW];
			}
		}
	}

	//�R���W�����f�[�^���C���[���u���v�̃f�[�^�ɕϊ�
	constexpr uint8_t tr_point = 1;//�������\�z����_(����)
	constexpr uint8_t tr_end = 2;//�������\�z����_(�E��I�_)
	constexpr uint8_t tr_point_end = 3;//�������\�z����_(����_�E���I�_)
	constexpr uint8_t tr_change_point = 4;//���ゾ���ǁA����̓_(��)�Ɠ���ւ���
	const auto collisionLayerNo = layer_no_collision;//�ς�邩������Ȃ�

	Position2f pendingPos= Position2f::NIL;
	Position2f lastPos = Position2f::NIL;//�����Ȑ��l
	terrainSegment_.clear();
	auto yoffset = ground_line - header_.mapH * header_.chipH * block_scale;
	for (size_t x = 0; x < header_.mapW; ++x) {
		for (size_t y = 0; y < header_.mapH; ++y) {
			auto data= stagedata_[collisionLayerNo][y + x * header_.mapH];
			if (data == 0)continue;
			auto pos = Position2f(static_cast<float>(x * header_.chipW),static_cast<float>(y * header_.chipH))
				*block_scale+Vector2f(0,yoffset);
			switch (data) {
			case tr_point://����̓_�f�[�^
				if (pendingPos.IsNil()) {
					CreateSegment(lastPos, pos);
				}
				else {
					CreateSegment(lastPos, pos);
					CreateSegment(pos, pendingPos);
					lastPos = pendingPos;
					pendingPos = Position2f::NIL;
				}
				break;
			case tr_end://�E��̏I�_�f�[�^
				pos += Vector2f(header_.chipW * block_scale, 0);
				CreateSegment(lastPos, pos);
				lastPos = Position2f::NIL;
				break;
			case tr_point_end://����E���̏I�_�f�[�^
				CreateSegment(lastPos, pos);
				pos += Vector2f(header_.chipW, header_.chipH)*block_scale;
				CreateSegment(lastPos, pos); 
				lastPos = Position2f::NIL;
				break;
			case tr_change_point://���̂Ɠ���ւ���
				pendingPos = pos;
				break;
			}
		}
	}
}

void Stage::CreateSegment(Position2f& startPos,const  Position2f& endPos)
{
	if (startPos.IsNil()) {
		terrainSegment_.push_back(Segment::NIL);
	}
	else {
		terrainSegment_.emplace_back(startPos, endPos - startPos);
	}
	startPos = endPos;
}

void 
Stage::Update() {

}
void 
Stage::BackDraw() {
	for (int i = 0; i < layer_no_front; ++i) {
		DrawChips(i);
	}
}

void Stage::DrawChips(size_t layerNo)
{
	
	const int yoffset = ground_line - static_cast<int>(static_cast<int>(header_.chipH) * header_.mapH * block_scale );
	const int xoffset =  static_cast<int>(camera_->ViewOffset().x);
	auto rc = camera_->GetViewRange();
	//��ŏ��������邪�܂��͕�����₷�����̂܂ܕ��ׂ�(�A���g��͂���)
	for (size_t x = 0; x < header_.mapW; ++x) {
		int xpos = static_cast<int>(x * header_.chipW * block_scale);
		int xmargin = static_cast<int>(header_.chipW * block_scale);
		if (xpos < rc.Left()- xmargin ||  rc.Right()+ xmargin<xpos) {
			continue;
		}
		for (size_t y = 0; y < header_.mapH; ++y) {
			auto chip = stagedata_[layerNo][y + x * header_.mapH];//�`�b�v�ԍ��擾
			DrawRectRotaGraph2(
				static_cast<int>(xoffset+x * header_.chipW * block_scale), //�\��X���W
				static_cast<int>(yoffset + y * header_.chipH * block_scale),//�\��Y���W
				(chip % 16) * header_.chipW,//���G�̍����W
				(chip / 16) * header_.chipH,//���G�̏���W
				header_.chipW,//���G�̕�
				header_.chipH, //���G�̍���
				0, 0,//���㒆�S�ɂ��Ă���
				block_scale,//�g�嗦
				0.0f,
				stageAtlasH_, true);
		}
	}
}

void
Stage::FrontDraw() {
	DrawChips(layer_no_front);
}

void 
Stage::DebugDraw() {
#ifdef _DEBUG
	auto& dbg = Debugger::Instance();
	if (!dbg.IsDebugMode())return;
	auto xoffset = camera_->ViewOffset().x;
	for (const auto& seg : terrainSegment_) {
		dbg.Draw(seg, 0xffffff, xoffset,0.0f , 3.0f);
	}
#endif
}

Size 
Stage::GetStageSize()const {
	return Size(static_cast<size_t>(header_.mapW * header_.chipW * block_scale), 
		static_cast<size_t>(header_.mapH * header_.chipH * block_scale));
}

Vector2f 
Stage::ComputeOverlapWall(const Position2f& pos,float r)const {
	auto it = find_if(terrainSegment_.begin(), terrainSegment_.end(), [pos,r](const Segment& seg) {
		if (seg.vec.IsNil())return false;
		if (seg.vec.x > 0.0f)return false;
		auto v = pos - seg.start;//�~�̒��S�ɑ΂���x�N�g��
		//�����̒���
		auto vlen = fabs(Cross( seg.vec.Normalized(),v));
		if (r < vlen)return false;
		//�ˉe�̒���
		auto srate= Dot( v, seg.vec)/ Dot(seg.vec, seg.vec);
		if (srate > 1.0f||srate<0.0f)return false;
		return true;
		});
	if (it == terrainSegment_.end()) {
		return Vector2f::ZERO;
	}
	//�����������Ƃ���܂ł̎ˉe�x�N�g�����v�Z
	auto v = pos - it->start;
	auto vv = it->vec * (Dot(v, it->vec) / Dot(it->vec, it->vec));
	auto n = (v - vv);
	auto mag = n.Magnitude();
	if (mag == 0.0f)return Vector2f::ZERO;
	return n.Normalized()* (r - mag);
}

float 
Stage::ComputeGroundY(const Position2f& pos)const {

	auto it = find_if(terrainSegment_.begin(), terrainSegment_.end(), [pos](const Segment& seg) {
		if (seg.vec.x == 0.0f)return false;
		const auto& spos = seg.start;
		auto epos = spos+seg.vec;
		return spos.x <= pos.x && pos.x <= epos.x;
		});
	if (it == terrainSegment_.end()) {
		return naraku_y;
	}	
	assert(it->vec.x > 0);
	float a = it->vec.y / it->vec.x;
	return it->start.y+a*(pos.x-it->start.x);
}

std::array<Segment, 3> 
Stage::GetThreeSegment(const Position2f& pos)const {
	std::array<Segment, 3> ret = { Segment::NIL,Segment::NIL ,Segment::NIL };
	auto it = find_if(terrainSegment_.begin(), terrainSegment_.end(), [pos](const Segment& seg) {
		if (seg.vec.x == 0.0f)return false;
		const auto& spos = seg.start;
		auto epos = spos + seg.vec;
		return spos.x <= pos.x && pos.x <= epos.x;
		});
	if (it == terrainSegment_.end()) {
		return ret;
	}
	ret[1] = *it;
	if (it != terrainSegment_.begin()) {
		ret[0]=*prev(it);
	}
	if (next(it) != terrainSegment_.end()) {
		ret[2] = *next(it);
	}
	return ret;
}

bool 
Stage::IsBossMode()const {
	constexpr uint8_t boss_no = 255;
	auto rc = camera_->GetViewRange();
	size_t xleft = static_cast<size_t>(static_cast<float>(rc.Left()) / (header_.chipW * block_scale));
	size_t xright = static_cast<size_t>(static_cast<float>(rc.Right()) / (header_.chipW * block_scale));
	auto itBegin = stagedata_[layer_no_enemy].begin();

	return count(
		next(itBegin, xleft * header_.mapH),//first
		next(itBegin, xright * header_.mapH),//last
		boss_no) > 0;
}