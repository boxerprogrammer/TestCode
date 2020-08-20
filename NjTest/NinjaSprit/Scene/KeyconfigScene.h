#pragma once
#include "Scene.h"
#include"../Geometry.h"
#include<string>
#include<vector>
#include"../Input/Input.h"

class GameplayingScene;
class PauseScene;
class KeyconfigScene :
    public Scene
{
	friend GameplayingScene;
	friend PauseScene;
private:
	int indicatorH_ = -1;
	int openSE_ = -1;
	int closeSE_ = -1;
	int moveSE_ = -1;
	int decideSE_ = -1;
	int cancelSE_ = -1;
	int changeSE_ = -1;
	size_t currentSelectNo_ = 0;
	std::vector<std::string> menuItems_;
	PeripheralReferenceTable_t peripheralReferenceTable_;
	Rect frameRect_ = { {200,100}, {500,400} };
	KeyconfigScene(SceneController& c ,const Vector2& offset);
	void CancelEdit(const Input& input);
	
	//初期化のためのUpdate
	//元のキーの割り当てなどを読み込む
	void InitializeUpdate(const Input&);
	//通常状態のUpdate
	void NormalUpdate(const Input&);
	//編集中のUpdate
	void EditUpdate(const Input&);
	

	using UpdateFunction_t = void (KeyconfigScene::*)(const Input&);
	UpdateFunction_t updater_;

	void NormalDraw();
	using DrawFunction_t = void(KeyconfigScene::*)();
	DrawFunction_t drawer_;
	void CloseMenu();
	void CommitCurrentKeyConfig(const Input& input);

	size_t titleW_;

public:
	~KeyconfigScene() = default;
	/// <summary>
	/// キーコンフィグシーン更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input & input)override;
	/// <summary>
	/// シーン描画
	/// </summary>
	void Draw()override;
};

