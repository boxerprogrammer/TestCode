#include "InputState.h"
#include<DxLib.h>
#include<algorithm>

using namespace std;

void 
InputState::Update() {
	copy(begin(keyState), end(keyState), begin(lastKeyState));
	GetHitKeyStateAll(keyState);

	ginput.input[static_cast<int>(GameInputValue::decide)]= keyState[KEY_INPUT_RETURN];

}