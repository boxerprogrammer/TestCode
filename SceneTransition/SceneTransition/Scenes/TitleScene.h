#pragma once
#include "Scene.h"
/// <summary>
/// �^�C�g���V�[���N���X
/// </summary>
class TitleScene : public Scene {
public:
	TitleScene(SceneManager& manager) :Scene(manager) {}
	virtual void Update(const InputState& input)override;
	virtual void Draw()override;
};
