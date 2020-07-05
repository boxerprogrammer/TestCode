#pragma once
#include"Scene.h"
#include<memory>
#include<vector>

class TitleScene;
class Player;
class Background;
class InputListener;
class GameplayingScene:public Scene 
{
	friend TitleScene;
private:
	GameplayingScene(SceneController&);

	std::unique_ptr<Player> player_;
	std::unique_ptr<Background> bg_;


	void FadeinUpdate(const Input&);
	void NormalUpdate(const Input&);
	void FadeoutUpdate(const Input&);

	using UpdateFunction_t = void (GameplayingScene::*)(const Input&);
	UpdateFunction_t updater_;



	void NormalDraw();
	void FadeDraw();
	void (GameplayingScene::* drawer_)();

	std::vector<std::shared_ptr<InputListener>> listeners_;

public:
	~GameplayingScene();
	void AddListener(std::shared_ptr<InputListener> listener);
	void Update(const Input&)override;
	void Draw()override;
};

