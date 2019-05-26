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

	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto vicviperH=LoadGraph("img/vicviper.png");

	array<int,20> screenH;
	for (auto& s : screenH) {
		s = MakeScreen(640, 480);
	}
	
	Position2 pos(320, 240);

	list<ControlPoint> ctrlPnts(4);

	int cnt = 0.;
	for (auto& p : ctrlPnts) {
		p.pos.x = pos.x + cnt * 50;
		p.pos.y = pos.y;// -i * 80 + (i*i) * 30;
		++cnt;
	}

	array<float, 3> lengthes;
	auto ctrlpnt0 = ctrlPnts.begin();
	auto ctrlpnt1 = ctrlpnt0;
	++ctrlpnt1;
	for (auto& l : lengthes) {
		l = (ctrlpnt1->pos - ctrlpnt0->pos).Length();
		++ctrlpnt0;
		++ctrlpnt1;
	}
	array<Position2f, 3> targetpositions;
	auto ctrlpnt = ctrlPnts.begin();
	++ctrlpnt;
	for (int i = 0; i < targetpositions.size(); ++i) {
		targetpositions[i] = ctrlpnt->pos;
		++ctrlpnt;
	}


	array<float, 3> velocities = { 15.0f,10.0f,10.0f };

	array<float, 3> omegas = { 0.25f,0.15f,0.1f };

	float angle =0.0f;
	auto lastangle = angle;
	char keystate[256];

	ctrlpnt = ctrlPnts.begin();
	vector<BendLazerPos> bs=CalculateCubicBezier(ctrlpnt->pos,
		(++ctrlpnt)->pos,
		(++ctrlpnt)->pos,
		(++ctrlpnt)->pos,
		10);
	
//	copy(bs.begin(), bs.end(), back_inserter(lazer));
	int frame = 0;
	int drawframe = 0;
	while (ProcessMessage()==0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);
		
		if (keystate[KEY_INPUT_UP]) {
			pos.y -= 2;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			pos.y += 2;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			pos.x += 2;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			pos.x -= 2;
		}

		if (keystate[KEY_INPUT_Z]) {
			angle += 1.0f/6.f;
			ctrlPnts.emplace_front( pos.ToFloatVec());
			frame = 0;
		}
		if (keystate[KEY_INPUT_X]) {
			angle -= 1.0f/6.f;
			ctrlPnts.emplace_front(pos.ToFloatVec());
			frame = 0;
		}

		++frame;


		//ターゲット座標設定
		auto length = 0.0f;
		for (int i = 0; i < targetpositions.size(); ++i) {
			length += lengthes[i];
			targetpositions[i] = pos.ToFloatVec() + Vector2f(cos(angle)*length, sin(angle)*length);
		}

		//線形補間
		//for (int i = 0; i < targetpositions.size(); ++i) {
		//	if ((ctrlPositions[i + 1] - targetpositions[i]).Length() < velocities[i]*1.6f) {
		//		ctrlPositions[i + 1] = targetpositions[i];
		//	}else{
		//		ctrlPositions[i + 1] += (targetpositions[i] - ctrlPositions[i + 1]).Normalized()*velocities[i];
		//	}
		//}
		if (ctrlPnts.empty()) {
			LoopEndProcess();
			continue;
		}
		ctrlpnt0 = ctrlPnts.begin();
		ctrlpnt1 = ctrlpnt0;
		++ctrlpnt1;



		//球面線形補間
		//for (int i = 0; i < targetpositions.size(); ++i) 
		{
/*			if ((ctrlPositions[i + 1] - targetpositions[i]).Length() < velocities[i]*1.2f) {
				ctrlPositions[i + 1] = targetpositions[i];
			}else*/{
				//線形補間座標

				//auto tmp = (ctrlpnt0->pos -pos.ToFloatVec()).Normalized();
				//auto tmp2 = (targetpositions[0]-pos.ToFloatVec()).Normalized();
				//auto cross = Cross(tmp, tmp2);
				//auto angle = sin(cross);
				//angle = copysign(min(omegas[0], abs(angle)), angle);
				//ctrlpnt0->pos = pos.ToFloatVec() + (tmp*Rotate(angle)) *lengthes[0];
				//tmp= ctrlpnt1->pos + (targetpositions[i] - ctrlpnt1->pos).Normalized()*velocities[i];
				//ctrlpnt1->pos = ctrlpnt0->pos + (tmp - ctrlpnt0->pos).Normalized()*lengthes[i];
			}
			//++ctrlpnt0;
			//++ctrlpnt1;
		}
		if (ctrlPnts.empty()) {
			LoopEndProcess();
			continue;
		}
		for (auto& cp : ctrlPnts) {
			if ((cp.pos - pos.ToFloatVec()).Length() == 0.0f) {
				cp.vel = Vector2f(cos(angle), sin(angle))*4;
			}
			else {
				cp.vel = (cp.pos - pos.ToFloatVec()).Normalized()*4;
			}
		}

		for (auto& cp : ctrlPnts) {
			cp.pos += cp.vel;
			cp.Update();
		}


		//for (auto& b : lazer) {
		//	if ((b.pos - pos.ToFloatVec() ).Length()==0.0f) {
		//		b.vel = Vector2f(cos(angle), sin(angle));
		//	}
		//	else {
		//		b.vel = (b.pos - pos.ToFloatVec()).Normalized();
		//	}
		//}

		//for (auto& b : lazer) {
		//	b.pos += b.vel;
		//}


		////コントロールポイント描画
		//for (const auto& p : ctrlPnts) {
		//	DrawCircle(p.pos.x, p.pos.y, 3, 0xffffff, true);
		//}

		//ctrlpnt0 = ctrlPnts.begin();
		//ctrlpnt1 = ctrlpnt0;
		//++ctrlpnt1;
		//DrawLine(pos.x, pos.y,
		//	ctrlpnt0->pos.x, ctrlpnt0->pos.y, 0xffffff, 2);
		//for (; ctrlpnt1 != ctrlPnts.end(); ++ctrlpnt0, ++ctrlpnt1) {
		//	if (ctrlpnt0->pos.x >= -120 && ctrlpnt0->pos.x <= 760 &&
		//		ctrlpnt0->pos.y >= -120 && ctrlpnt0->pos.y <= 600 &&
		//		ctrlpnt1->pos.x >= -120 && ctrlpnt1->pos.x <= 760 &&
		//		ctrlpnt1->pos.y >= -120 && ctrlpnt1->pos.y <= 600) {
		//		DrawLine(ctrlpnt0->pos.x, ctrlpnt0->pos.y,
		//			ctrlpnt1->pos.x, ctrlpnt1->pos.y, 0xffffff, 2);
		//	}

		//}

		//不必要なコントロールポイントを削除
		ctrlPnts.erase( std::remove_if(ctrlPnts.begin(), ctrlPnts.end(), [](const ControlPoint& pred) {
			int x = pred.pos.x;
			int y = pred.pos.y;
			return x < -200 || x>840 || y < -200 || y>680 || pred.life<0;
			}),ctrlPnts.end()) ;

		list<BendLazerPos> lazer;
		//ベジェ曲線計算(今回はコントロールポイントすべて通したいため)
		//中間点は自前で計算する
		//std::list<ControlPoint>::reverse_iterator lastit = ctrlPnts.rbegin();
		if (ctrlPnts.size() <= 2) {
			LoopEndProcess();
			continue;
		}
		auto cpitend = ctrlPnts.rend();
		--cpitend;
		--cpitend;
		--cpitend;
		for (auto cpit = ctrlPnts.rbegin(); cpit != cpitend; ++cpit) {



			auto p0 = cpit == ctrlPnts.rbegin() ? pos.ToFloatVec() : cpit->pos;
			auto cpit1 = cpit;
			auto p1 = cpit == ctrlPnts.rbegin() ? cpit->pos : (++cpit1)->pos;
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
			auto mp0 = mpos0+(mpos1 - mpos0)*m0;
			auto mp1 = mpos1+(mpos2 - mpos1)*m2;

			mp0 = mpos1 - mp0 + p1;
			mp1 = mpos1 - mp1 + p2;


			//auto cp3 = cpit;
			//++cp3;
			//if (cp3 == ctrlPnts.rend())break;

			std::vector<BendLazerPos> bz;
			//if (lastit == cp0) {//一番最初
				bz = CalculateCubicBezier(p1,
					mp0,
					mp1,
					p2,
					10);
			//}
			//else {
				//bz = CalculateCubicBezier(lastit->pos,
				//	cp0->pos,
				//	cp1->pos,
				//	(cp2 == ctrlPnts.rend()) ? pos.ToFloatVec() : cp2->pos,
				//	10);
			//}
			
			//lastit = cp2;
			std::copy(bz.begin(), bz.end(), back_inserter(lazer));
		}
			//auto rit = bz.rbegin();
			//for (; rit != bz.rend(); ++rit) {
			//	lazer.push_front(*rit);
			//}
		//	
		//}
		//for (auto& b : lazer) {
		//	if ((b.pos - pos.ToFloatVec() ).Length()==0.0f) {
		//		b.vel = Vector2f(cos(angle), sin(angle));
		//	}
		//	else {
		//		b.vel = (b.pos - pos.ToFloatVec()).Normalized();
		//	}
		//}

		//for (auto& b : lazer) {
		//	b.pos += b.vel;
		//}

		//ベジェ描画
		//for (const auto& p : lazer) {
		//	if ( p.pos.x > 0 && p.pos.x <= 640 && p.pos.y > 0 && p.pos.y <= 480) {
		//		DrawCircle(p.pos.x, p.pos.y, 10, 0x88aaff, true);
		//	}
		//}
		if (lazer.empty()) {
			LoopEndProcess();
			continue;
		}

		SetDrawScreen(screenH[drawframe]);
		ClearDrawScreen();
		auto it = lazer.begin();
		it++;
		for (; it!=lazer.end();++it) {
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
		DrawLine(pos.x, pos.y, lazer.back().pos.x, lazer.back().pos.y, 0x88aaff, 3);
		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, screenH[drawframe], true);
		for (int i = 0; i < screenH.size(); ++i) {
			int idx = (drawframe + i) % screenH.size();
			SetDrawBlendMode(DX_BLENDMODE_ADD,26*(i+1));
			GraphFilter(screenH[idx], DX_GRAPH_FILTER_GAUSS, 8, 100);
			DrawGraph(0, 0, screenH[idx], true);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		}
		
		DrawRotaGraph(pos.x, pos.y, 2.0f, 0.0f, vicviperH, true);

		lastangle = angle;

		drawframe = (drawframe + 1) % screenH.size();


		LoopEndProcess();
	}

	return 0;
}