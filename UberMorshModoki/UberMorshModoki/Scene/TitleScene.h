#pragma once
#include "Scene.h"
/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene : public Scene {
private:
	using UpdateFunc_t=void (TitleScene::* )();
	using DrawFunc_t = void (TitleScene::*)();
	void BeginUpdate() {};
	void BeginDraw() {};
	void MainUpdate() {};
	void MainDraw() {};
	void EndUpdate() {};
	void EndDraw() {};
	UpdateFunc_t updateFunc_ = &TitleScene::BeginUpdate;
	UpdateFunc_t drawFunc_ = &TitleScene::BeginDraw;
	int fadeTime_ = 0;
public:
	TitleScene(SceneManager& manager) :Scene(manager) {}
	~TitleScene() {}
	virtual void Update(const InputState& input)override;
	virtual void Draw()override;
};
