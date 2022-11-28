#pragma once

#include"../Geometry/Geometry.h"
#include<map>
#include<vector>
#include<functional>
enum class GameInputCode {
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
	bool input[static_cast<int>(GameInputCode::none)];
};

struct AnalogState {
	float LTrigger;
	float RTrigger;
	float LX,LY,RX,RY;
};

enum class InputCategory {
	keyboard,
	mouse,
	gamepad
};
struct InputState {
	InputCategory category;
	int inputCode;
	InputState(InputCategory cat, int code);
};
class Input{
	char keyState[256];
	int padState;
	int mouseInput;
	Vector2 mousePos;
	AnalogState analogState;
	GameInput gameInput;
	GameInput lastInput;
	std::map<GameInputCode, std::vector<InputState>> inputMap_;
	std::array<std::function<bool > [static_cast<int>(GameInputCode::none)]
	void RegisterCode(GameInputCode code, const std::vector<Input>& input);
public:
	Input();
	~Input() {};
	void Update();
	bool IsTrigger(GameInputCode code)const;
	bool IsPressed(GameInputCode code)const;
};
