#include<DxLib.h>
#include<cassert>
#include<list>

using namespace std;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();

	SetDrawScreen(DX_SCREEN_BACK);
	int bloodH[5] = {};
	wchar_t path[64];
	for (int i = 1; i <= 5; ++i) {
		wsprintf(path, L"./Asset/Blood/blood%d.png", i);
		bloodH[i-1] = LoadGraph(path);
		assert(bloodH[i-1] >= 0);
	}

	struct KeyState {
		char state[256];
	};
	KeyState currentKeystate = {};
	KeyState lastKeystate = {};
	int mouseInput=0;
	int lastMouseInput=0;
	int splattingTime = 0;
	constexpr int splatter_interval = 18;
	constexpr int div = splatter_interval / 18;
	struct Blood {
		int x = -1;
		int y = -1;
		int time = 0;
		bool isTurn = false;
		int h = 0;
	};
	std::list<Blood> bloods = {};
	while (ProcessMessage()!=-1)
	{
		GetHitKeyStateAll(currentKeystate.state);
		int mx, my;
		GetMousePoint(&mx, &my);
		mouseInput=GetMouseInput();
		if (!(lastMouseInput&MOUSE_INPUT_LEFT) && (mouseInput & MOUSE_INPUT_LEFT)&&splattingTime==0) {
			bloods.push_back({ mx,my,splatter_interval ,rand()%2==0,bloodH[rand()%5]});
		}

		for (auto& b : bloods) {
			if (b.time > 0) {

				int idx = (splatter_interval - b.time) / div;
				int idxX = idx % 6;
				int idxY = idx / 6;
				DrawRectRotaGraph(b.x, b.y, idxX * 100, idxY * 100, 100, 100, 2.0, 0.0, b.h,true, b.isTurn);
				--b.time;
			}
		}
		bloods.remove_if([](auto& b) {
			return b.time == 0;
			});
		
		ScreenFlip();
		lastMouseInput = mouseInput;
		lastKeystate = currentKeystate;
	}
	DxLib_End();
}