#pragma once
#include "Scene.h"
class GamePlayingScene : public Scene {
public:
	GamePlayingScene(SceneManager& manager) :Scene(manager) {}
	virtual void Update(const InputState& input)override;
	virtual void Draw()override;
};
