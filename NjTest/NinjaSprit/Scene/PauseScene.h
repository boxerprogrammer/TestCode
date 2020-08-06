#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<vector>
#include<string>
#include<functional>
class GameplayingScene;
class PauseScene :
	public Scene
{
	friend GameplayingScene;
private:
	struct MenuItem {
		std::wstring menuText;
		Position2 pos;
		bool isActive = false;
		std::function<void(void)> func;
		MenuItem(const wchar_t* str, const Position2& p,  std::function<void(void)> f):
			menuText(str),func(f),pos(p){}
	};
	std::vector<MenuItem> menuItems_;

	PauseScene(SceneController& c);
	~PauseScene();


	void AppearUpdate(const Input&);
	void NormalUpdate(const Input&);
	void CloseMenu();
	void DisppearUpdate(const Input&);
	using UpdateFunction_t = void (PauseScene::*)(const Input&);
	UpdateFunction_t updater_;

	void AppearDraw();

	void NormalDraw();
	using DrawFunction_t = void (PauseScene::*)();
	DrawFunction_t drawer_;
public:
	void Update(const Input&)override;
	void Draw()override;
};

