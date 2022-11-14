#pragma once
#include "Scene.h"
class BeforeGameScene : public Scene {
public:
	virtual void Update(const InputState& input)override;
	virtual void Draw()override;
};
