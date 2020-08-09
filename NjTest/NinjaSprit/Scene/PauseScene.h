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
	int indicatorH_ = -1;
	int openSE_ = -1;
	int closeSE_ = -1;
	int moveSE_ = -1;
	int decideSE_ = -1;
	int cancelSE_ = -1;
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
	/// <summary>
	/// ポーズシーン更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;
	/// <summary>
	/// ポーズシーン描画
	/// </summary>
	void Draw()override;
};

