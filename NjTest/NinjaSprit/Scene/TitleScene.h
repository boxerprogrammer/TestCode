#pragma once
#include"Scene.h"
class GameoverScene;
class PauseScene;
class TitleScene:public Scene 
{
	friend SceneController;
	friend GameoverScene;
	friend PauseScene;
private:
	TitleScene(SceneController& c);//ファクトリにしか作らせたくない
	int titleH_=-1;
	int startH_=-1;
	int startSE_ = -1;
	float angle_=3.14f;//テスト用
	float dist_ = 0.0f;//テスト用ゆがみパラメータ
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
	/// <summary>
	/// タイトルシーン更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;
	/// <summary>
	/// タイトルシーン描画
	/// </summary>
	void Draw()override;
};



