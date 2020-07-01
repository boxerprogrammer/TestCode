#pragma once
#include"Scene.h"
class TitleScene;
class GameplayingScene:public Scene 
{
	friend TitleScene;
private:
	GameplayingScene(SceneController&);
	void FadeinUpdate(const Input&);
	void NormalUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (GameplayingScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	void FadeDraw();
	void (GameplayingScene::* drawer_)();
public:
	~GameplayingScene() = default;
	void Update(const Input&)override;
	void Draw()override;
};

