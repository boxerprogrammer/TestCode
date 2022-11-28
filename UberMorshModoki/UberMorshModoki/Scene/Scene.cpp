#include "Scene.h"
#include"../input/InputState.h"
bool 
Scene::IsTriggered(const InputState& input, char key) {
	return true;/*!input.lastKeyState[key] &&
		input.keyState[key];*/
}