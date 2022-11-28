#include "InputState.h"
#include<DxLib.h>
#include<algorithm>

using namespace std;

void Input::RegisterCode(GameInputCode code, const std::vector<Input>& input)
{
}

Input::Input()
{
	inputMap_ = { {GameInputCode::decide,{
		{InputCategory::gamepad,PAD_INPUT_R},
		{InputCategory::keyboard,KEY_INPUT_RETURN},
		{InputCategory::mouse,MOUSE_INPUT_LEFT}}} 
	};
}

void
Input::Update() {
	lastInput = gameInput;//ゲーム入力ステートのコピー
	for (const auto& info : inputMap_) {
		auto& code = gameInput.input[static_cast<int>(info.first)];
		for (const auto& state : info.second) {
			//state.category
		}
	}

}

InputState::InputState(InputCategory cat, int code):category(cat),inputCode(code)
{
}
