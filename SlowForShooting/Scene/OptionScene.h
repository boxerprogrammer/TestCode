#pragma once
#include "Scene.h"
class InputState;

///�I�v�V�����V�[��
class OptionScene : public Scene {
public:
	OptionScene(SceneManager& manager) :Scene(manager) {};
	~OptionScene() {}

	void Update(const InputState& input);
	void Draw();
};

