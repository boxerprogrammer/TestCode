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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	Position2 pos(320, 240);

	array<Position2f, 4> ctrlPositions;

	for (int i = 0; i < ctrlPositions.size(); ++i) {
		ctrlPositions[i].x = pos.x + i * 20;
		ctrlPositions[i].y = pos.y;// -i * 80 + (i*i) * 30;
	}

	array<float, 3> lengthes;
	for (int i = 0; i < lengthes.size(); ++i) {
		lengthes[i] = (ctrlPositions[i + 1] - ctrlPositions[i]).Length();
	}
	array<Position2f, 3> targetpositions;
	for (int i = 0; i < targetpositions.size(); ++i) {
		targetpositions[i] =ctrlPositions[i+1];
	}


	array<float, 3> velocities = { 15.0f,10.0f,10.0f };

	array<float, 3> omegas = { 0.25f,0.15f,0.1f };

	float angle =0.0f;
	auto lastangle = angle;
	char keystate[256];


	vector<BendLazerPos> bs=CalculateCubicBezier(ctrlPositions[0],
		ctrlPositions[1],
		ctrlPositions[2],
		ctrlPositions[3],
		20);
	list<BendLazerPos> lazer;
	copy(bs.begin(), bs.end(), back_inserter(lazer));
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
			angle += 0.12f;
		}
		if (keystate[KEY_INPUT_X]) {
			angle -= 0.12f;
		}

		ctrlPositions[0] = pos.ToFloatVec();

		//ターゲット座標設定
		auto length = 0.0f;
		for (int i = 0; i < targetpositions.size(); ++i) {
			length += lengthes[i];
			targetpositions[i] = ctrlPositions[0] + Vector2f(cos(angle)*length, sin(angle)*length);
		}

		//線形補間
		//for (int i = 0; i < targetpositions.size(); ++i) {
		//	if ((ctrlPositions[i + 1] - targetpositions[i]).Length() < velocities[i]*1.6f) {
		//		ctrlPositions[i + 1] = targetpositions[i];
		//	}else{
		//		ctrlPositions[i + 1] += (targetpositions[i] - ctrlPositions[i + 1]).Normalized()*velocities[i];
		//	}
		//}

		//球面線形補間
		for (int i = 0; i < targetpositions.size(); ++i) {
/*			if ((ctrlPositions[i + 1] - targetpositions[i]).Length() < velocities[i]*1.2f) {
				ctrlPositions[i + 1] = targetpositions[i];
			}else*/{
				//線形補間座標
				auto tmp = (ctrlPositions[i + 1] -ctrlPositions[i]).Normalized();
				auto tmp2 = (targetpositions[i]-ctrlPositions[i]).Normalized();
				auto cross = Cross(tmp, tmp2);
				auto angle = sin(cross);
				angle = copysign(min(omegas[i], abs(angle)), angle);
				ctrlPositions[i + 1] = ctrlPositions[i] + (tmp*Rotate(angle))*lengthes[i];
				//auto tmp=ctrlPositions[i + 1] + (targetpositions[i] - ctrlPositions[i + 1]).Normalized()*velocities[i];				
				//ctrlPositions[i + 1] = ctrlPositions[i] + (tmp - ctrlPositions[i]).Normalized()*lengthes[i];
			}
		}

		//コントロールポイント描画
		//for (const auto& p : ctrlPositions) {
		//	DrawCircle(p.x, p.y, 3, 0xffffff, true);
		//}
		//for (int i = 1; i < ctrlPositions.size();++i) {
		//	const auto& p = ctrlPositions[i];
		//	const auto& prep = ctrlPositions[i-1];
		//	DrawLine(prep.x, prep.y, p.x, p.y, 0xffffff,2);
		//}

		//ベジェ曲線計算
		
		if (lastangle != angle) {
			auto bz = CalculateCubicBezier(ctrlPositions[0],
				ctrlPositions[1],
				ctrlPositions[2],
				ctrlPositions[3],
				20);
			copy(bz.begin(), bz.end(), front_inserter(lazer));
		}
		for (auto& b : lazer) {
			if ((b.pos - pos.ToFloatVec() ).Length()==0.0f) {
				b.vel = Vector2f(cos(angle), sin(angle));
			}
			else {
				b.vel = (b.pos - pos.ToFloatVec()).Normalized();
			}
		}

		for (auto& b : lazer) {
			b.pos += b.vel;
		}

		//ベジェ描画
		for (const auto& p : lazer) {
			DrawCircle(p.pos.x, p.pos.y, 1, 0x88aaff, true);
		}
		auto it = lazer.begin();
		it++;
		for (; it!=lazer.end();++it) {
			const auto& p = *it;
			auto it2 = it;
			--it2;
			const auto& prep = *it2;
			DrawLine(prep.pos.x, prep.pos.y, p.pos.x, p.pos.y, 0x88aaff,2);
		}
		
		DrawCircle(pos.x, pos.y, 5, 0xff0000, false,3);


		lastangle = angle;

		ScreenFlip();
	}

	return 0;
}