#include<vector>
#include<array>
#include<cassert>
#include<DxLib.h>
#include"Geometry.h"
using namespace std;

vector<Position2f> CalculateCubicBezier(const Position2f& pos1, 
	const Position2f& pos2, 
	const Position2f& pos3, 
	const Position2f& pos4,
	unsigned int div) {
	assert(div > 4);
	vector<Position2f> ret;
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
		ctrlPositions[i].x = pos.x + i * 100;
		ctrlPositions[i].y = pos.y - i*80+(i*i)*30;
	}

	array<float, 3> lengthes;
	for (int i = 0; i < lengthes.size(); ++i) {
		lengthes[i] = (ctrlPositions[i + 1] - ctrlPositions[0]).Length();
	}
	array<Position2f, 3> targetpositions;
	for (int i = 0; i < targetpositions.size(); ++i) {
		targetpositions[i] =ctrlPositions[i+1];
	}


	array<float, 3> velocities = { 15.0f,15.0f,15.0f };

	float angle = 0.0f;
	char keystate[256];
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
			angle += 0.1f;
		}
		if (keystate[KEY_INPUT_X]) {
			angle -= 0.1f;
		}

		ctrlPositions[0] = pos.ToFloatVec();

		for (int i = 0; i < targetpositions.size(); ++i) {
			targetpositions[i] = ctrlPositions[0] + Vector2f(cos(angle)*lengthes[i], sin(angle)*lengthes[i]);
		}

		for (int i = 0; i < targetpositions.size(); ++i) {
			if ((ctrlPositions[i + 1] - targetpositions[i]).Length() < velocities[i]*1.6f) {
				ctrlPositions[i + 1] = targetpositions[i];
			}else{
				ctrlPositions[i + 1] += (targetpositions[i] - ctrlPositions[i + 1]).Normalized()*velocities[i];
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
		auto bz=CalculateCubicBezier(ctrlPositions[0],
			ctrlPositions[1],
			ctrlPositions[2],
			ctrlPositions[3],
			20);

		//ベジェ描画
		for (const auto& p : bz) {
			DrawCircle(p.x, p.y, 1, 0x88aaff, true);
		}
		for (int i = 1; i < bz.size();++i) {
			const auto& p = bz[i];
			const auto& prep = bz[i - 1];
			DrawLine(prep.x, prep.y, p.x, p.y, 0x88aaff);
		}
		
		DrawCircle(pos.x, pos.y, 5, 0xff0000, false,3);




		ScreenFlip();
	}

	return 0;
}