#include<DxLib.h>
#include<array>
#include<algorithm>

using namespace std;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_  LPSTR, _In_ int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	auto grad=LoadGraph(L"./grad.png");
	constexpr int note_width = 80;
	constexpr int note_height = 30;
	constexpr int note_min_x = 0;// 160;
	constexpr int note_line_top = 0;// 50;
	constexpr int note_line_bottom = 450;// 450;
	constexpr int check_bar_y = 350;//400
	constexpr int note_num = 4;
	int note_speed = 4;
	float lerp = 1.0f;
	auto rt=MakeScreen(note_width * note_num+20, note_line_bottom, true);

	struct Note {
		int y = -note_height;
		int x = 0;
		bool isEnable = false;
	};

	std::array<Note,64> notes = {};

	const int beat_interval = 15;
	int beat_count = 0;
	VERTEX3D ver = { { -1,0,10},{0,1,0},{255,255,255,255},{0,0,0,0},0,0,0,0 };
	VERTEX3D verts[4] = {
		{{ -10,0,10},{0,1,0},{255,255,255,255},{0,0,0,0},0,0,0,0},
		{{ 10,0,10},{0,1,0},{255,255,255,255},{0,0,0,0},0,0,0,0},
		{{ -10,0,0},{0,1,0},{255,255,255,255},{0,0,0,0},0,0,0,0},
		{{ 10,0,0},{0,1,0},{255,255,255,255},{0,0,0,0},0,0,0,0}
	};
	DxLib::SetCameraPositionAndTargetAndUpVec(VGet(0, 2, 0), VGet(0, 0, 10), VGet(0, 1, 0));
	SetCameraNearFarD(0.5, 50.0f);
	unsigned short int indices[6] = { 0,1,2,1,3,2 };
	

	while (ProcessMessage() != -1) {
		//SetDrawScreen(rt);
		//ClearDrawScreen();

		//if (++beat_count % beat_interval == 0) {
		//	for (int i = 0; i < note_num; ++i) {
		//		if (rand() % 4 == 0) {
		//			auto it = std::find_if(notes.begin(), notes.end(), [](const Note& n) {
		//				return !n.isEnable;
		//				});
		//			it->isEnable = true;
		//			it->y= -note_height;
		//			it->x = note_min_x + i * note_width;

		//			
		//		}
		//	}
		//}

		//for (auto& n : notes) {
		//	if (n.isEnable) {
		//		float lerp = static_cast<float>(n.y) / 450.0f;
		//		DrawBox(n.x, n.y,
		//			n.x + note_width,
		//			n.y + note_height*lerp, 0xaaccff,true);
		//		n.y += note_speed;
		//		if (n.y > note_line_bottom) {
		//			n.isEnable = false;
		//		}
		//	}
		//}
		//
		//DrawBox(note_min_x, check_bar_y,
		//	note_min_x + note_num * note_width, check_bar_y + note_height,
		//	0xffaaaa, true);

		//for (int i = 0; i < note_num+1; ++i) {
		//	DrawLine(note_min_x + i * note_width,
		//		note_line_top,
		//		note_min_x + i * note_width,
		//		note_line_bottom,
		//		0xffffff, 3);
		//}
		//
		//SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
		//DrawModiGraph(0, 0, 
		//	note_width* note_num + 20, 0,
		//	note_width* note_num + 20,note_line_bottom, 
		//	0, note_line_bottom, grad, false);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


		SetDrawScreen(DX_SCREEN_BACK);
		//ClearDrawScreen();
		//DrawModiGraph(320 - 20, 50,
		//	320 + 20, 50,
		//	320 + 220, 450,
		//	320 - 220, 450, rt, false);
		//GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 8, 800);
		//SetDrawBlendMode(DX_BLENDMODE_ADD, 168);
		//DrawModiGraph(320 - 20, 50,
		//	320 + 20, 50,
		//	320 + 220, 450,
		//	320 - 220, 450, rt, false);
		//GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 16, 800);
		//DrawModiGraph(320 - 20, 50,
		//	320 + 20, 50,
		//	320 + 220, 450,
		//	320 - 220, 450, rt, false);
		//GraphFilter(rt, DX_GRAPH_FILTER_GAUSS, 32, 1600);
		//DrawModiGraph(320 - 20, 50,
		//	320 + 20, 50,
		//	320 + 220, 450,
		//	320 - 220, 450, rt, false);
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		//DrawPolygonIndexed3D(verts, 4, indices, 2, rt, true);
		DrawCube3D(VGet(-5, 0, 0), VGet(5, -10, 10), 0xffaaaa, 0xffffff, true);
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}