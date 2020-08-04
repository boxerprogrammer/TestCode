#include "Stage.h"
#include"../Geometry.h"
#include"Camera.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>
#include"../System/Debugger.h"
#include<limits>
#include<array>

using namespace std;

namespace {
	int stageAtlasH_ = -1;
	constexpr float block_scale = 2.0f;
	constexpr int ground_line = 600;
	constexpr float naraku_y = FLT_MAX;
	constexpr uint8_t layer_no_back0 = 0;//背景(奥)
	constexpr uint8_t layer_no_back1 = 1;//背景(手前)
	constexpr uint8_t layer_no_front = 2;//前景(プレイヤーの前)
	constexpr uint8_t layer_no_collision = 3;//地形コリジョン
	constexpr uint8_t layer_no_enemy = 4;//敵配置(255はボス)
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

	//コリジョンデータレイヤーを「線」のデータに変換
	constexpr uint8_t tr_point = 1;//線分を構築する点(左上)
	constexpr uint8_t tr_end = 2;//線分を構築する点(右上終点)
	constexpr uint8_t tr_point_end = 3;//線分を構築する点(左上点右下終点)
	constexpr uint8_t tr_change_point = 4;//左上だけど、直後の点(下)と入れ替える
	const auto collisionLayerNo = layer_no_collision;//変わるかもしれない

	Position2f pendingPos= Position2f::NIL;
	Position2f lastPos = Position2f::NIL;//無効な数値
	terrainSegment_.clear();
	auto yoffset = ground_line - header_.mapH * header_.chipH * block_scale;
	for (size_t x = 0; x < header_.mapW; ++x) {
		for (size_t y = 0; y < header_.mapH; ++y) {
			auto data= stagedata_[collisionLayerNo][y + x * header_.mapH];
			if (data == 0)continue;
			auto pos = Position2f(static_cast<float>(x * header_.chipW),static_cast<float>(y * header_.chipH))
				*block_scale+Vector2f(0,yoffset);
			switch (data) {
			case tr_point://左上の点データ
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
			case tr_end://右上の終点データ
				pos += Vector2f(header_.chipW * block_scale, 0);
				CreateSegment(lastPos, pos);
				lastPos = Position2f::NIL;
				break;
			case tr_point_end://左上右下の終点データ
				CreateSegment(lastPos, pos);
				pos += Vector2f(header_.chipW, header_.chipH)*block_scale;
				CreateSegment(lastPos, pos); 
				lastPos = Position2f::NIL;
				break;
			case tr_change_point://次のと入れ替える
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
	//後で書き換えるがまずは分かりやすくそのまま並べる(但し拡大はする)
	for (size_t x = 0; x < header_.mapW; ++x) {
		int xpos = static_cast<int>(x * header_.chipW * block_scale);
		int xmargin = static_cast<int>(header_.chipW * block_scale);
		if (xpos < rc.Left()- xmargin ||  rc.Right()+ xmargin<xpos) {
			continue;
		}
		for (size_t y = 0; y < header_.mapH; ++y) {
			auto chip = stagedata_[layerNo][y + x * header_.mapH];//チップ番号取得
			DrawRectRotaGraph2(
				static_cast<int>(xoffset+x * header_.chipW * block_scale), //表示X座標
				static_cast<int>(yoffset + y * header_.chipH * block_scale),//表示Y座標
				(chip % 16) * header_.chipW,//元絵の左座標
				(chip / 16) * header_.chipH,//元絵の上座標
				header_.chipW,//元絵の幅
				header_.chipH, //元絵の高さ
				0, 0,//左上中心にしている
				block_scale,//拡大率
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
		auto v = pos - seg.start;//円の中心に対するベクトル
		//垂線の長さ
		auto vlen = fabs(Cross( seg.vec.Normalized(),v));
		if (r < vlen)return false;
		//射影の長さ
		auto srate= Dot( v, seg.vec)/ Dot(seg.vec, seg.vec);
		if (srate > 1.0f||srate<0.0f)return false;
		return true;
		});
	if (it == terrainSegment_.end()) {
		return Vector2f::ZERO;
	}
	//垂線下したところまでの射影ベクトルを計算
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