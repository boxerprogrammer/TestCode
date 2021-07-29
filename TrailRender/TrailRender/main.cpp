#include<DxLib.h>
#include"Geometry.h"
#include<list>


using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	list<Position2> trail;

	constexpr size_t trail_limit = 10;

	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		int mx, my;
		GetMousePoint(&mx, &my);
		DrawCircleAA(mx, my, 15, 16, 0xffaaaa);
		Position2 lastpos(mx, my);
		float thickness = 30.0f;
		for (auto& pos : trail) {
			DrawLineAA(lastpos.x, lastpos.y, pos.x, pos.y, 0xffffff, thickness);
			thickness /= 1.2;
			lastpos = pos;
		}
		trail.push_front(Position2(mx, my));
		if (trail.size() >= trail_limit) {
			trail.pop_back();
		}
		ScreenFlip(); 
	}

	DxLib_End();
	return 0;
}