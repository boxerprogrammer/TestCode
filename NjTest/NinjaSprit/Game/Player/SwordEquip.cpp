#include "SwordEquip.h"
#include"../Player/Player.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include"../Camera.h"
#include"ShadowClone.h"
#include<DxLib.h>
#include<vector>
using namespace std;
namespace {
	constexpr float equip_offset_y = -60.0f;
	constexpr int swing_frame = 30;
	constexpr int swing_frame_div2 = swing_frame/2;
	int pixelShaderH_ = -1;
	int screenH_ = -1;
	float w_ = 800;
	float h_ = 600;

};
SwordEquip::SwordEquip(std::shared_ptr<Player>& p, std::shared_ptr<CollisionManager> cm, std::shared_ptr<Camera> camera, ShadowClone* shadow) :player_(p),
Equipment(cm, camera),
shadow_(shadow){
	if (pixelShaderH_==-1) {
		pixelShaderH_=LoadPixelShader(L"Resource/Etc/testps.pso");
	}
	auto& fileMgr = FileManager::Instance();
	normalH_ = fileMgr.Load(L"Resource/Etc/normal.png")->Handle();
	if (screenH_ == -1) {
		SetUseDivGraphFlag(false);
		screenH_ = MakeScreen(w_, h_);
	}
	
}

void
SwordEquip::Attack(const Player& player, const Input& input, Vector2f offset) {
	swingFrame_ = swing_frame;
	auto pos = player_->GetPosition();
	const auto xoffset = camera_->ViewOffset().x;
	pos += Vector2f(0, equip_offset_y);
	slash_.center.x = pos.x + xoffset;

	slash_.v1 = slash_.v2=Vector2f(-125, -125);
	swingTargetAngle_ = 2.0f*DX_PI_F / 3.0f;
	swingPerAngle_ = 2.0f*swingTargetAngle_/(float)swingFrame_;
	currentAngle_ = 0.0f;
}

void
SwordEquip::AdditionalInput(const Input& input) {

}

void
SwordEquip::Update() {
	if (swingFrame_ <= 0)return;
	auto pos = player_->GetPosition();
	const auto xoffset = camera_->ViewOffset().x;
	pos += Vector2f(0, equip_offset_y);
	slash_.center = pos;
	slash_.center.x = pos.x + xoffset;
	if (swingFrame_ >= swing_frame_div2) {
		currentAngle_ += swingPerAngle_;
		slash_.AddAngle2(swingPerAngle_);
	}
	else {
		currentAngle_ -= swingPerAngle_;
		slash_.AddAngle1(swingPerAngle_);
	}
	--swingFrame_;
}

void
SwordEquip::Draw() {
	
	if (swingFrame_ > 0) {
		auto pos = player_->GetPosition();
		if (shadow_ != nullptr) {
			pos = shadow_->GetPosition();
		}
		const auto xoffset = camera_->ViewOffset().x;
		pos += Vector2f(0, equip_offset_y);
		slash_.center.x = pos.x+xoffset;
		if (pixelShaderH_ > 0) {
			if (shadow_ == nullptr) {
				GetDrawScreenGraph(0, 0, w_, h_, screenH_);
			}
			slash_.Draw(screenH_,pixelShaderH_,normalH_);
		}
		else {
			//DX_NONE_GRAPH;
			GetDrawScreenGraph(0, 0, w_, h_, screenH_);
			slash_.Draw(screenH_);
		}
	}
}

Slash::Slash(const Position2f& p, const Vector2f& inv1, const Vector2f& inv2) :
	center(p), v1(inv1), v2(inv2) {

}
Slash::Slash(const Position2f& p, float r, float angle) : center(p) {
	v1 = Vector2f(r, 0);
	v2 = Vector2f(r * cos(angle), r * sin(angle));
}
Slash::Slash(const Position2f& p, const Vector2f& inv, float angle) :
	center(p), v1(inv) {
	v2 = RotateMat(angle) * v1;
}
float
Slash::GetAngle()const {
	return GetAngle2Vector(v1, v2);
}

void
Slash::AddAngle1(float angle) {
	float tmpAngle = GetAngle2Vector(v2, v1);
	v1 = RotateMat(tmpAngle + angle) * v2;
}

void
Slash::AddAngle2(float angle) {
	float tmpAngle = GetAngle2Vector(v1, v2);
	v2 = RotateMat(tmpAngle + angle) * v1;
}


float
Slash::Radius()const {
	return v1.Magnitude();
}

void
Slash::Draw( int graphH,int psH,int normalH,unsigned int color) {
	constexpr float min_angle = DX_PI_F / 36.0f;//だいたい5度くらい
	float angle = GetAngle2Vector(v1, v2);

	size_t triangles_num = (size_t)ceil(angle / min_angle);
	++triangles_num;
	vector<VERTEX2DSHADER> v;
	vector<VERTEX2D> ngV;
	float r = v1.Magnitude();
	float dr = r / (float)triangles_num;
	auto vstart = v1;
	if (psH==-1) {
		
		ngV.resize(2 * (size_t)triangles_num);
		for (auto& e : ngV) {
			e.rhw = 1.0f;
			e.dif = GetColorU8(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff, 255);
			e.u = 0;
			e.v = 0;
		}

		auto vstart = v1;
		for (size_t i = 0; i < triangles_num - 1; ++i) {
			auto p = center + vstart.Normalized() * r;
			ngV[i * 2 + 0].pos = V2V(p);
			ngV[i * 2 + 0].u = (p.x / w_)+cos((float)i / (float)triangles_num) / 20.0f;
			ngV[i * 2 + 0].v = (p.y / h_)+sin((float)i / (float)triangles_num) / 20.0f;
			v[i * 2 + 0].dif.r = 16;
			v[i * 2 + 0].dif.g = 64;
			v[i * 2 + 0].dif.b = 192;
			v[i * 2 + 0].dif.a = 32;
			p = center + vstart;
			ngV[i * 2 + 1].pos = V2V(p);
			ngV[i * 2 + 1].u = (p.x / w_)+cos((float)(i + 2) / (float)triangles_num) / 50.0f;
			ngV[i * 2 + 1].v = (p.y / h_)+sin((float)(i + 2) / (float)triangles_num) / 50.0f;
			v[i * 2 + 1].dif.r = 192;
			v[i * 2 + 1].dif.g = 192;
			v[i * 2 + 1].dif.b = 255;
			v[i * 2 + 1].dif.a = 255;
			vstart = RotateMat(min_angle) * vstart;
			r -= dr;
		}
		//最後の三角形
		size_t idx = ((size_t)triangles_num - 1) * 2;
		auto pos = center + v2.Normalized() * r;
		ngV[idx].pos = V2V(pos);
		ngV[idx].u = pos.x / w_;
		ngV[idx].v = pos.y / h_;
		ngV[idx].dif.r = 168;
		ngV[idx].dif.g = 168;
		ngV[idx].dif.b = 255;
		ngV[idx].dif.a = 192;
		pos = center + v2;
		ngV[idx + 1].pos = V2V(pos);
		ngV[idx + 1].u = pos.x / w_;
		ngV[idx + 1].v = pos.y / h_;
		ngV[idx + 1].dif.r = 255;
		ngV[idx + 1].dif.g = 255;
		ngV[idx + 1].dif.b = 255;
		ngV[idx + 1].dif.a = 255;
	}
	else {
		v.resize(2 * (size_t)triangles_num);

		for (size_t i = 0; i < triangles_num - 1; ++i) {
			auto p = center + vstart.Normalized() * r;
			v[i * 2 + 0].pos = V2V(p);
			v[i * 2 + 0].u = (p.x / w_)+cos((float)i / (float)triangles_num) / 20.0f;
			v[i * 2 + 0].v = (p.y / h_)+sin((float)i / (float)triangles_num) / 20.0f;
			v[i * 2 + 0].rhw = 1.0f;
			v[i * 2 + 0].dif.r = 16;
			v[i * 2 + 0].dif.g = 64;
			v[i * 2 + 0].dif.b = 192;
			v[i * 2 + 0].dif.a = 32;
			p = center + vstart;
			v[i * 2 + 1].pos = V2V(p);
			v[i * 2 + 1].u = (p.x / w_)+cos((float)(i + 2) / (float)triangles_num) / 50.0f;
			v[i * 2 + 1].v = (p.y / h_)+sin((float)(i + 2) / (float)triangles_num) / 50.0f;
			v[i * 2 + 1].rhw = 1.0f;
			v[i * 2 + 1].dif.r = 192;
			v[i * 2 + 1].dif.g = 192;
			v[i * 2 + 1].dif.b = 255;
			v[i * 2 + 1].dif.a = 255;
			vstart = RotateMat(min_angle) * vstart;
			r -= dr;
		}
		//最後の三角形
		size_t idx = ((size_t)triangles_num - 1) * 2;
		auto pos = center + v2.Normalized() * r;
		v[idx].pos = V2V(pos);
		v[idx].u = pos.x / w_;
		v[idx].v = pos.y / h_;
		v[idx].rhw = 1.0f;
		v[idx].dif.r = 168;
		v[idx].dif.g = 168;
		v[idx].dif.b = 255;
		v[idx].dif.a = 192;
		pos = center + v2;
		v[idx + 1].pos = V2V(pos);
		v[idx + 1].u = pos.x / w_;
		v[idx + 1].v = pos.y / h_;
		v[idx + 1].rhw = 1.0f;
		v[idx + 1].dif.r = 255;
		v[idx + 1].dif.g = 255;
		v[idx + 1].dif.b = 255;
		v[idx + 1].dif.a = 255;
	}

	

	if(psH==-1){
		DrawPrimitive2D(ngV.data(), ngV.size(), DX_PRIMTYPE_TRIANGLESTRIP, graphH, true);
	}
	else {
		// ２Ｄポリゴンを描画する
		// 使用するピクセルシェーダーをセット
		SetUseTextureToShader(0, graphH);
		SetUseTextureToShader(1, normalH);
		SetUsePixelShader(psH);
		DrawPrimitive2DToShader(v.data(), v.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	}

}
