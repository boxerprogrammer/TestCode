#pragma once
#include"Scene.h"
class GameplayingScene;
class ContinueScene:public Scene 
{
	friend GameplayingScene;
private:
	ContinueScene(SceneController&);
	void FadeinUpdate(const Input&);
	void WaitUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (ContinueScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	void FadeDraw();
	void (ContinueScene::* drawer_)();
public:
	~ContinueScene() = default;
	void Update(const Input&)override;
	void Draw()override;
};

