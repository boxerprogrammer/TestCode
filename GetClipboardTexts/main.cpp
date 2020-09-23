#include<DxLib.h>
#include<string>
#include<list>
#include<sstream>
#include<algorithm>
#include<random>

using namespace std;

TCHAR clip[4096] = {};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	random_device rd;
	srand(rd());
	mt19937 mt(rd());
	uniform_int_distribution<int> int_dist(0,10);
	ChangeWindowMode(true);
	int sw=960, sh=540;
	SetGraphMode(sw, sh, 32);
	SetWindowText(L"ÅöÇ®äyÇµÇ›íäëIâÔÉrÉìÉSÇ≠ÇÒÅö");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	int slotframe=LoadGraph(L"img/slot_frame.png");
	int bg = LoadGraph(L"img/bg.png");
	int shuchuH = LoadGraph(L"img/shuchu.png");
	int drumrollSE = LoadSoundMem(L"se/drum-roll.mp3");
	int janSE = LoadSoundMem(L"se/jean.mp3");
	int fanfareSE = LoadSoundMem(L"se/fanfare.mp3");
	int mitokeyoSE = LoadSoundMem(L"se/mitokeyo.wav");
	int yattazeSE[3];
	yattazeSE[0] = LoadSoundMem(L"se/yattaze0.mp3");
	yattazeSE[1] = LoadSoundMem(L"se/yattaze1.mp3");
	yattazeSE[2] = LoadSoundMem(L"se/yattaze2.wav");
	int gonSE = LoadSoundMem(L"se/gon.mp3");

	char keystate[256] = {};
	char lastkeystate[256] = {};
	list<wstring> namelist;
	size_t frame = 0;

	int sf_w, sf_h;
	constexpr int rot_speed_1 = 20;
	constexpr int rot_speed_2 = 30;
	constexpr int start_y = 40;
	constexpr int start_x = 200;
	constexpr int name_start_x = start_x +50;
	constexpr int max_name_count = 6;
	constexpr int name_width = 200;
	constexpr int name_height = 80;
	constexpr int fanfare_name_height = 120;
	constexpr int reel_start_x = 200;
	constexpr int reel_start_y = start_y;
	constexpr int reel_width = name_width + 400;
	constexpr int reel_height = name_height * max_name_count;

	int reelRT = MakeScreen(reel_width, reel_height);
	
	SetFontSize(name_height);
	GetGraphSize(slotframe, &sf_w, &sf_h);
	
	int captureH = MakeScreen(sw, sh);

	size_t reelRotFrame1 = 0;
	size_t reelRotFrame2 = 0;
	size_t rotSpeed1 = 0;
	size_t rotSpeed2 = 0;

	size_t stopFrame = 0;
	bool blnStop = false;
	bool blnRotate = false;
	bool blnDecided = false;
	bool blnFanfare = false;
	bool blnYattaze = false;
	bool gon1 = false;
	bool gon2 = false;
	bool suberi1 = false;
	bool suberi2 = false;
	int currentYattazeSE = 0;
	wstring hitName=L"";
	while (ProcessMessage() != -1) {
		//ó†âÊñ Ç…ï`âÊ
		SetDrawScreen(DX_SCREEN_BACK);
		GetHitKeyStateAll(keystate);
		if ((keystate[KEY_INPUT_V] && keystate[KEY_INPUT_LCONTROL]) &&
			!(lastkeystate[KEY_INPUT_V] && lastkeystate[KEY_INPUT_LCONTROL])) {
			DxLib::GetClipboardText(clip);
			namelist.clear();
			wstringstream ss;
			ss << clip;
			wstring name;
			while (std::getline(ss, name)) {
				namelist.push_back(name);
			}
			int_dist = uniform_int_distribution<int>(0, namelist.size() - 1);
			blnRotate = false;
			gon1 = false;
			gon2 = false;
		}

		if (keystate[KEY_INPUT_SPACE] && !lastkeystate[KEY_INPUT_SPACE] && !blnFanfare) {
			if (blnRotate) {
				blnStop = true;
				rotSpeed1 = 5;
				rotSpeed2 = 10;
				StopSoundMem(drumrollSE);
				hitName = *next(namelist.begin(), int_dist(mt));
				currentYattazeSE = yattazeSE[rand() % 3];
			}
			else {
				if (!namelist.empty()) {
					blnRotate = true;
					rotSpeed1 = rot_speed_1;
					rotSpeed2 = rot_speed_2;
					gon1 = false;
					gon2 = false;
					blnStop = false;
					suberi1 = false;
					suberi2 = false;
					hitName = L"";
					stopFrame = 0;
					PlaySoundMem(mitokeyoSE, DX_PLAYTYPE_BACK);
					PlaySoundMem(drumrollSE, DX_PLAYTYPE_LOOP);
				}
			}
		}

		if (blnFanfare) {
			int dw, dh, lc;
			GetDrawStringSize(&dw, &dh, &lc, hitName.c_str(), hitName.length());
			if (dw > sw) {
				SetFontSize(name_height);
				GetDrawStringSize(&dw, &dh, &lc, hitName.c_str(), hitName.length());
			}
			int nx = (sw - dw) / 2;
			int ny = (sh - dh) / 2;
			
			DrawBox(0, 0, sw, sh, 0xffffff, true);
			DrawExtendGraph(0, 0, sw, sh, shuchuH, true);
			DrawString(nx, ny, hitName.c_str(), 0x000000);

			
			if (!CheckSoundMem(fanfareSE) &&!blnYattaze) {
				PlaySoundMem(currentYattazeSE, DX_PLAYTYPE_BACK);
				blnYattaze = true;
			}

			if (keystate[KEY_INPUT_SPACE] && !lastkeystate[KEY_INPUT_SPACE]) {
				blnStop = false;
				blnRotate = false;
				blnDecided = false;
				blnFanfare = false;
				blnYattaze = false;
				gon1 = false;
				gon2 = false;
				suberi1 = false;
				suberi2 = false;
				hitName = L"";
				SetFontSize(name_height);
			}
		}
		else {
			//ÉäÅ[ÉãÇÉäÅ[ÉãRTÇ…ï`âÊ
			SetDrawScreen(reelRT);
			int y = 0;
			int x = 0;
			int line_height = name_height / 4;
			int h = reel_height / line_height;
			float t = 0.0f;
			float rate = DX_PI_F / float(h);
			int reel_y = 0;
			DrawBox(x, 0, reel_width, reel_height, 0xffffee, true);
			for (int i = 0; i < h; ++i) {
				float wgt = 0.75f + 0.25f * sin(t);
				t += rate;
				reel_y += line_height;
			}
			for (auto& name : namelist) {
				DrawString(20 + x, y, name.c_str(), 0x444444);
				y += name_height;
				if (y >= reel_height)break;
			}

			//ó†âÊñ Ç…ï`âÊ
			SetDrawScreen(DX_SCREEN_BACK);


			DrawRectGraph(reel_start_x, reel_start_y,
				0, 0,
				reel_width / 3, reel_height, reelRT, false);

			int reel_div_width = 190;
			DrawRectGraph(reel_start_x + reel_div_width, reel_start_y + reelRotFrame1,
				reel_div_width, 0,
				reel_div_width, reel_height, reelRT, false);
			DrawRectGraph(reel_start_x + reel_div_width, reel_start_y + reelRotFrame1 - reel_height,
				reel_div_width, 0,
				reel_div_width, reel_height, reelRT, false);

			DrawRectGraph(reel_start_x + reel_div_width * 2, reel_start_y + reelRotFrame2 % reel_height,
				reel_div_width * 2, 0,
				reel_div_width, reel_height, reelRT, false);
			DrawRectGraph(reel_start_x + reel_div_width * 2, reel_start_y + reelRotFrame2 % reel_height - reel_height,
				reel_div_width * 2, 0,
				reel_div_width, reel_height, reelRT, false);

			DrawGraph(0, 0, bg, true);
			if (blnRotate) {
				frame = (frame + 1) % 2;
				reelRotFrame1 = (reelRotFrame1 + rotSpeed1) % reel_height;
				reelRotFrame2 = (reelRotFrame2 + rotSpeed2) % reel_height;
				if (!blnStop || *next(namelist.begin(), 2) != hitName) {
					if (frame == 0) {
						if (!namelist.empty()) {
							std::rotate(namelist.begin(), next(namelist.begin()), namelist.end());
						}
						if (*next(namelist.begin(), 2) == hitName) {
							PlaySoundMem(gonSE, DX_PLAYTYPE_BACK);
						}
					}
				}
				else {
					if (reelRotFrame1 == 0) {
						if (suberi1) {
							rotSpeed1 = 0;
							if (rotSpeed2 > 0 && !gon2) {
								gon2 = true;
								PlaySoundMem(gonSE, DX_PLAYTYPE_BACK);
							}
						}
						else {
							suberi1 = true;
						}
					}
					if (reelRotFrame2 == 0) {
						if (suberi2) {
							rotSpeed2 = 0;
							if (rotSpeed1 > 0 && !gon1) {
								gon1 = true;
								PlaySoundMem(gonSE, DX_PLAYTYPE_BACK);
							}
						}
						else {
							suberi2 = true;
						}
					}
					if (rotSpeed1 == 0 && rotSpeed2 == 0) {
						blnRotate = false;
						PlaySoundMem(janSE, DX_PLAYTYPE_BACK);
						blnDecided = true;
					}
				}
			}
			DrawRotaGraph(sw / 2, sh / 2, 1.5f, 0.0f, slotframe, true);

			if (blnDecided && !CheckSoundMem(janSE)) {
				blnFanfare = true;
				SetFontSize(fanfare_name_height);
				GetDrawScreenGraph(0, 0, sw, sh, captureH);
				PlaySoundMem(fanfareSE, DX_PLAYTYPE_BACK);
			}
		}

		ScreenFlip();
		copy(begin(keystate), end(keystate), begin(lastkeystate));
		
	}

}