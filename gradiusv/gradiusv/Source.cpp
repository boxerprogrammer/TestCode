#include<vector>
#include<list>
#include<array>
#include<cassert>
#include<DxLib.h>
#include"Geometry.h"
using namespace std;

struct BendLazerPos {
	Position2f pos=Position2f(100,100);
	Vector2f vel = Vector2f(0, 0);
	BendLazerPos() {}
	BendLazerPos(const Position2f& p) :pos(p) {}
	BendLazerPos(const Position2f& p,const Vector2f& v) :pos(p),vel(v) {}
};

struct ControlPoint {
	Position2f pos = Position2f(100, 100);
	Vector2f vel = Vector2f(0, 0);
	 int life = 600;
	ControlPoint() {}
	ControlPoint(const Position2f& p) :pos(p) {}
	ControlPoint(const Position2f& p, const Vector2f& v) :pos(p), vel(v) {}
	void Update() { life--; }
	bool isDead() { life >= 0; }
};

vector<BendLazerPos> CalculateCubicBezier(const Position2f& pos1,
	const Position2f& pos2, 
	const Position2f& pos3, 
	const Position2f& pos4,
	unsigned int div) {
	assert(div > 4);
	vector<BendLazerPos> ret;
	ret.reserve(div);
	for (int i = 0; i < div; ++i) {
		auto t = static_cast<float>(i) / static_cast<float>(div);
		auto r = 1.0f - t;
		auto a = r * r*r;
		auto b = 3 * t*r*r;
		auto c = 3 * t*t*r;
		auto d = t * t*t;
		
		ret.emplace_back(pos1*a + pos2 * b + pos3 * c + pos4 * d);
	}
	return ret;
}

void LoopEndProcess() {
	DrawFormatString(10, 20, 0xffffff, "FPS=%f", GetFPS());
	DrawFormatString(10, 40, 0xffffff, "DrawCall=%d", GetDrawCallCount());

	ScreenFlip();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int optionnum = 3;
	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto vicviperH=LoadGraph("img/vicviper.png");
	auto optionH = LoadGraph("img/option.png");

	

	array<int,10> screenH;
	for (auto& s : screenH) {
		s = MakeScreen(640, 480);
	}
	
	vector<Position2f> poss(optionnum+1);
	for (auto& pos : poss) {
		pos = Vector2f(320, 240);
	}
	
	vector<list<ControlPoint>> ctrlPnts(optionnum + 1);
	for (int i = 0; i < poss.size();++i) {
		int cnt = 0;
		ctrlPnts[i].resize(4);
		for (auto& cp : ctrlPnts[i]) {
			cp.pos.x=poss[i].x + cnt * 50;
			++cnt;
		}
	}

	array<float, 3> velocities = { 15.0f,10.0f,10.0f };

	array<float, 3> omegas = { 0.25f,0.15f,0.1f };

	float angle =0.0f;
	auto lastangle = angle;
	char keystate[256];
	char lastkeystate[256];

	vector<vector<BendLazerPos>> bss(optionnum + 1);

	for (int i = 0; i < ctrlPnts.size(); ++i) {
		
		auto ctrlpnt = ctrlPnts[i].begin();
		bss[i] = CalculateCubicBezier(ctrlpnt->pos,
			(++ctrlpnt)->pos,
			(++ctrlpnt)->pos,
			(++ctrlpnt)->pos,
			10);
		
	}
	
//	copy(bs.begin(), bs.end(), back_inserter(lazer));
	int frame = 0;
	int drawframe = 0;
	const float maxoptdistance = 80.0f;
	while (ProcessMessage()==0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);
		
		bool isChanged = false;
		auto& pos = poss[0];
		if (keystate[KEY_INPUT_UP]) {
			pos.y -= 4;
			isChanged = true;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			pos.y += 4;
			isChanged = true;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			pos.x += 4;
			isChanged = true;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			pos.x -= 4;
			isChanged = true;
		}

		if (keystate[KEY_INPUT_Z]) {
			angle += 1.0f/6.f;
			isChanged = true;
		}
		if (keystate[KEY_INPUT_X]) {
			angle -= 1.0f/6.f;
			isChanged = true;
			
		}
		if (isChanged ) {
			for (int i = 0; i < poss.size(); ++i) {
				auto& cps = ctrlPnts[i];
				cps.emplace_front(poss[i].ToFloatVec());
				
			}
		}
		
		Position2f oplstPos = pos.ToFloatVec();
		for (int i = 1; i < poss.size();++i) {
			auto& o = poss[i];
			auto len = (o - oplstPos).Length();
			o= oplstPos+(o - oplstPos).Normalized()*min(len,maxoptdistance);
			oplstPos = o;
		}

		



		if (ctrlPnts.empty()) {
			LoopEndProcess();
			continue;
		}

		if (ctrlPnts.empty()) {
			LoopEndProcess();
			continue;
		}
		for (auto& cps : ctrlPnts) {
			for (auto& cp : cps) {
				if (cp.vel == Vector2f()) {
					cp.vel = Vector2f(cos(angle), sin(angle)) * 10;
				}
			}
		}

		for (auto& cps : ctrlPnts) {
			for (auto& cp : cps) {
				cp.pos += cp.vel;
				cp.Update();
			}
		}

		for (auto& cps : ctrlPnts) {
			//不必要なコントロールポイントを削除
			cps.erase(std::remove_if(cps.begin(), cps.end(), [](const ControlPoint& pred) {
				int x = pred.pos.x;
				int y = pred.pos.y;
				return x < -200 || x>840 || y < -200 || y>680 || pred.life < 0;
				}), cps.end());
		}

		vector<list<BendLazerPos>> lazer(optionnum+1);
		//ベジェ曲線計算(今回はコントロールポイントすべて通したいため)
		//中間点は自前で計算する
		for (int idx = 0; idx < ctrlPnts.size();++idx) {
			auto& cps = ctrlPnts[idx];
			if (cps.size() <= 2) {
				LoopEndProcess();
				continue;
			}
			auto cpitend = cps.rend();
			--cpitend;
			--cpitend;
			--cpitend;
			for (auto cpit = cps.rbegin(); cpit != cpitend; ++cpit) {
				auto p0 = cpit == cps.rbegin() ? poss[idx].ToFloatVec() : cpit->pos;
				auto cpit1 = cpit;
				auto p1 = cpit == cps.rbegin() ? cpit->pos : (++cpit1)->pos;
				auto cpit2 = cpit1;
				auto p2 = (++cpit2)->pos;
				auto cpit3 = cpit2;
				auto p3 = (++cpit3)->pos;

				if (p0.x < -120 || p0.x > 760 ||
					p0.y < -120 || p0.y > 600 ||
					p1.x < -120 || p1.x > 760 ||
					p1.y < -120 || p1.y > 600 ||
					p2.x < -120 || p2.x > 760 ||
					p2.y < -120 || p2.y > 600 ||
					p3.x < -120 || p3.x > 760 ||
					p3.y < -120 || p3.y > 600
					) {
					continue;
				}

				//中間点といっても中点では、長い辺と短い辺の差別化はできないため
				//割合を計算しておく
				//まず長さを計算
				auto len0 = (p1 - p0).Length();
				auto len1 = (p2 - p1).Length();
				auto len2 = (p3 - p2).Length();

				//割合を計算(m1はm0もしくはm2が分かれば自動的にわかるため必要ない)
				auto m0 = len0 + len1 == 0.0f ? 0.0f : len0 / (len0 + len1);
				auto m2 = len1 + len2 == 0.0f ? 0.0f : len1 / (len1 + len2);

				//中点計算
				auto mpos0 = (p0 + p1)*0.5f;
				auto mpos1 = (p1 + p2)*0.5f;
				auto mpos2 = (p2 + p3)*0.5f;

				//中間点計算
				auto mp0 = mpos0 + (mpos1 - mpos0)*m0;
				auto mp1 = mpos1 + (mpos2 - mpos1)*m2;

				mp0 = mpos1 - mp0 + p1;
				mp1 = mpos1 - mp1 + p2;


				std::vector<BendLazerPos> bz;
				bz = CalculateCubicBezier(p1,
					mp0,
					mp1,
					p2,
					10);
				std::copy(bz.begin(), bz.end(), back_inserter(lazer[idx]));
			}
		}
			
		SetDrawScreen(screenH[drawframe]);
		ClearDrawScreen();
		for (int i = 0; i < lazer.size(); ++i) {
			if (lazer[i].empty()) {//レーザーない時は相手にしない。
				continue;
			}
			auto it = lazer[i].begin();
			it++;
			for (; it != lazer[i].end(); ++it) {
				const auto& p = *it;
				auto it2 = it;
				--it2;
				const auto& prep = *it2;
				if (prep.pos.x > 0 && prep.pos.x <= 640 && p.pos.x > 0 && p.pos.x <= 640 &&
					prep.pos.y > 0 && prep.pos.y <= 480 && p.pos.y > 0 && p.pos.y <= 480) {
					DrawLine(prep.pos.x, prep.pos.y, p.pos.x, p.pos.y, 0x88aaff, 3);
				}
			}

			////
			DrawLine(poss[i].x, poss[i].y, lazer[i].back().pos.x, lazer[i].back().pos.y, 0x88aaff, 3);
		}
		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, screenH[drawframe], true);
		for (int i = 0; i < screenH.size(); ++i) {
			int idx = (drawframe + i) % screenH.size();
			SetDrawBlendMode(DX_BLENDMODE_ADD,20*(i+1));
			GraphFilter(screenH[idx], DX_GRAPH_FILTER_GAUSS, 8, 100);
			DrawGraph(0, 0, screenH[idx], true);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		}
		
		for (int i = 1; i < poss.size();++i) {
			auto& o = poss[i];
			DrawRotaGraph(o.x, o.y, 2.0f, 0.0f, optionH, true);
		}

		DrawRotaGraph(pos.x, pos.y, 2.0f, 0.0f, vicviperH, true);

		lastangle = angle;

		drawframe = (drawframe + 1) % screenH.size();
		
		copy(begin(keystate), end(keystate), begin(lastkeystate));

		LoopEndProcess();
	}

	return 0;
}