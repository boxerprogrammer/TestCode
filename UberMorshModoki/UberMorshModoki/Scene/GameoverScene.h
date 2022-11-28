#pragma once
#include "Scene.h"
class GameoverScene : public Scene {
public:
	GameoverScene(SceneManager& manager) :Scene(manager) {}
	virtual void Update(const InputState& input)override;
	virtual void Draw()override;
};