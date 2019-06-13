#include<DxLib.h>
#include<iostream>
#include"input.h"

int main() {
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SetGraphMode(1280, 720, 32);
	DxLib::ChangeWindowMode(true);
	DxLib_Init();
	XINPUT_STATE xstate;
	auto joypadnum=DxLib::GetJoypadNum();
	
	Input input;
	input.AddCommand(0, "up", 0, KEY_INPUT_UP);
	input.AddCommand(0, "down", 0, KEY_INPUT_DOWN);
	input.AddCommand(0, "right", 0, KEY_INPUT_RIGHT);
	input.AddCommand(0, "left", 0, KEY_INPUT_LEFT);
	input.AddCommand(0, "jump", 0, KEY_INPUT_Z);
	input.AddCommand(0, "attack", 0, KEY_INPUT_X);

	auto padcount = input.GetConnectedPadCount();
	for (int i = 0; i < padcount; ++i) {
		input.AddCommand(i, "up", i+1, PAD_INPUT_UP);
		input.AddCommand(i, "down", i+1, PAD_INPUT_DOWN);
		input.AddCommand(i, "right", i + 1, PAD_INPUT_RIGHT);
		input.AddCommand(i, "left", i + 1, PAD_INPUT_LEFT);
		input.AddCommand(i, "jump", i + 1, PAD_INPUT_1);
		input.AddCommand(i, "attack", i + 1, PAD_INPUT_2);
	}
	while (ProcessMessage()==0) {
		ClearDrawScreen();
		input.Update();
		int row = 0;
		int idx = 0;
		auto inputstate = input.CurrentState();
		for (auto& m : inputstate) {
			for (auto& s : m) {
				DrawFormatString(50, 50 + row * 25, 0xffffff, "PNO=%d , INPUT=%s , INPUT_CODE=%d", idx,s.first.c_str(),s.second);
				row++;
			}
			++idx;
		}
		//for (int i = 0; i <= joypadnum; ++i) {
		//	auto pdstate=GetJoypadInputState(i);
		//	int x, y;
		//	
		//	GetJoypadAnalogInput(&x, &y, i);
		//	GetJoypadXInputState(i, &xstate);
		//	DrawFormatString(50, 50 + i * 50, 0xffffff, "PAD=%d , INPUT_CODE=%x", i, pdstate);
		//	DrawFormatString(50, 250 + i * 50, 0xffffff, "PAD=%d , analogueX=%d , analogueY=%d", i, x,y);
		//	DrawFormatString(50, 400 + i * 50, 0xffffff, "PAD=%d , ltrigger=%d , rtrigger=%d", i, xstate.LeftTrigger, xstate.RightTrigger);
		//}
		ScreenFlip();
	}
	DxLib_End();
}