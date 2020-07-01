#pragma once
#include"Scene.h"
class GameoverScene;
class PauseScene;
class TitleScene:public Scene 
{
	friend PauseScene;
	friend SceneController;
	friend GameoverScene;
private:
	TitleScene(SceneController& c);//ファクトリにしか作らせたくない

	void FadeinUpdate(const Input&);
	void WaitUpdate(const Input&);
	void BlinkUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (TitleScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	void FadeDraw();
	void (TitleScene::*drawer_)();

public:
	~TitleScene();
	void Update(const Input&)override;
	void Draw()override;
};



