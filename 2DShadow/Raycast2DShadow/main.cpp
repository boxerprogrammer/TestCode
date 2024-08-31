#include<DxLib.h>
#include<vector>
#include<algorithm>
#include"Geometry.h"


using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetWindowText(L"Raycast2DShadow");
	ChangeWindowMode(true);
	if (DxLib::DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	std::vector<Rect> rects;
	rects.emplace_back(200, 100, 100, 50);
	rects.emplace_back(600, 200, 50, 100);
	rects.emplace_back(500, 300, 50, 50);
	rects.emplace_back(150, 300, 150, 100);
	constexpr float shadow_length = 1000.0f;


	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		int mx, my;
		GetMousePoint(&mx, &my);

		auto mousePos = Position2(mx, my);
		DrawCircleAA(mx, my, 500, 32, 0x888800);
		DrawCircleAA(mx, my, 240, 32, 0xdddd88);
		DrawCircleAA(mx, my, 16, 16, 0xffffaa);

		for (auto& rc : rects) {
			auto v = rc.Center()- mousePos;
			v.Normalize();
			DrawLineAA(mx, my, rc.Center().x, rc.Center().y,0xffffff );
			auto verts = rc.GetPositions();
			std::sort(verts.begin(), verts.end(), [v, mousePos](const Position2& a, const Position2& b) {
				return Cross(v, (a-mousePos).Normalized()) > Cross(v, (b-mousePos).Normalized());
			});

			auto v1= verts.front() - mousePos;
			auto v2 = verts.back() - mousePos;
			v1.Normalize();
			v2.Normalize();
			auto p1 = verts[0];
			auto p2 = verts[0] + v1* shadow_length;
			auto p3 = verts[3] + v2* shadow_length;
			auto p4 = verts[3];
			DrawQuadrangleAA(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, 0x000000, true);


			DrawLineAA(mx, my, verts[0].x, verts[0].y, 0xff0000);
			DrawLineAA(mx, my, verts[3].x, verts[3].y, 0xff0000);
		}
		for (auto& rc : rects) {
			rc.Draw(0x00ff00,true);
		}

		ScreenFlip();
	}
	DxLib_End();

	return 0;
}