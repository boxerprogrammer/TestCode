#pragma once
#include"Scene.h"
class ContinueScene;
class GameoverScene:public Scene 
{
	friend ContinueScene;
private:
	GameoverScene(SceneController&);
	void FadeinUpdate(const Input&);
	void WaitUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (GameoverScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	void FadeDraw();
	void (GameoverScene::* drawer_)();
public:
	~GameoverScene() = default;
	void Update(const Input&)override;
	void Draw()override;
};

