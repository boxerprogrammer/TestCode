#include "Scene.h"
#include"../input/InputState.h"
bool 
Scene::IsTriggered(const InputState& input, char key) {
	return !input.lastKeyState[key] &&
		input.keyState[key];
}