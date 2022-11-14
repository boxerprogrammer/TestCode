#pragma once

enum class GameInputValue {
	up,
	down,
	left,
	right,
	attack,
	jump,
	decide,
	cancel,
	none,
};
struct GameInput {
	bool input[static_cast<int>(GameInputValue::none)];
};

struct InputState {
	char keyState[256];
	char lastKeyState[256];
	GameInput ginput;
	void Update();
};
